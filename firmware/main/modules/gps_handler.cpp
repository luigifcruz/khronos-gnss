#include "gps_handler.h"

char *readLine(uart_port_t uart) {
    static char line[256];
    int size;
    char *ptr = line;
    while(1) {
        size = uart_read_bytes(uart, (unsigned char *)ptr, 1, portMAX_DELAY);
        if (size == 1) {
            if (*ptr == '\n') {
                ptr++;
                *ptr = 0;
                return line;
            }
            ptr++;
        }
    }
}

void GpsHandler::ParseNMEA(char* line, void* pvParameters, time_t* then) {
    static Database *db = (Database*)pvParameters;
    State s = db->GetState();

    switch (minmea_sentence_id(line, false)) {
        case MINMEA_SENTENCE_GGA: {
            struct minmea_sentence_gga frame;
            if (minmea_parse_gga(&frame, line)) {
                s.gnss_fix_quality = frame.fix_quality;
                s.latitude = minmea_tocoord(&frame.latitude);
                s.longitude = minmea_tocoord(&frame.longitude);
                s.altitude = minmea_tofloat(&frame.altitude);
            }
        } break;

        case MINMEA_SENTENCE_VTG: {
            struct minmea_sentence_vtg frame;
            if (minmea_parse_vtg(&frame, line)) {
                s.true_north = minmea_tofloat(&frame.true_track_degrees);
                s.ground_speed = minmea_tofloat(&frame.speed_kph);
            }
        } break;

        case MINMEA_SENTENCE_GSA: {
            struct minmea_sentence_gsa frame;
            if (minmea_parse_gsa(&frame, line)) {
                s.gnss_fix_type = frame.fix_type;
            }
        } break;

        case MINMEA_SENTENCE_GSV: {
            struct minmea_sentence_gsv frame;
            if (minmea_parse_gsv(&frame, line)) {
                if (strstr(line, "GLGSV")) {
                    s.sat_count_glonass = frame.total_sats;
                }

                if (strstr(line, "GPGSV")) {
                    s.sat_count_gps = frame.total_sats;
                }
            }
        } break;

        case MINMEA_SENTENCE_ZDA: {
            struct minmea_sentence_zda frame;
            if (minmea_parse_zda(&frame, line)) {
                time_t now;
                struct tm tm;
                time(&now);
                localtime_r(&now, &tm);
                tm.tm_year = frame.date.year - 1900;
                tm.tm_mon = frame.date.month - 1;
                tm.tm_mday = frame.date.day;
                tm.tm_hour = frame.time.hours - frame.hour_offset;
                tm.tm_min = frame.time.minutes - frame.minute_offset;
                tm.tm_sec = frame.time.seconds;

                if (now-*then > 60 || now < 50) {
                    const struct timeval tv = {mktime(&tm), 0};
                    settimeofday(&tv, 0);
                    *then = now;

                    char strftime_buf[64];
                    strftime(strftime_buf, sizeof(strftime_buf), "%FT%TZ", &tm);
                    ESP_LOGI(CONFIG_SN, "[GPS] Time updated to %s", strftime_buf);
                }
            }
        } break;

        default: break;
    }

    db->UpdateState(s);
}

void GpsHandler::ProgramUBX() {
    const char ubx_enable_zda[] = {0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x5a, 0x44, 0x41, 0x2a, 0x33, 0x39, 0x0d, 0x0a, 0xb5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x08, 0x01, 0x03, 0x20};
    uart_write_bytes(UART_NUM_2, (const char *)&ubx_enable_zda, sizeof(ubx_enable_zda));

    //const char ubx_10hz_update[] = {0xb5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x01, 0x00, 0x7A, 0x12};
    //uart_write_bytes(UART_NUM_2, (const char *)&ubx_10hz_update, sizeof(ubx_10hz_update));

    //const char ubx_enable_fast[] = {0xb5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xd0, 0x08, 0x00, 0x00, 0x00, 0xc2, 0x01, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc4, 0x96, 0xb5, 0x62, 0x06, 0x00, 0x01, 0x00, 0x01, 0x08, 0x22};
    //uart_write_bytes(UART_NUM_2, (const char *)&ubx_enable_fast, sizeof(ubx_enable_fast));
    //uart_set_baudrate(UART_NUM_2, 115200);

    ESP_LOGI(CONFIG_SN, "[GPS] UBX module programmed.");
}

void GpsHandler::GpsChannel(void* pvParameters) {
    uart_config_t uart_config;
    time_t then = 0;
    
    uart_config.baud_rate = 9600;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity    = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.rx_flow_ctrl_thresh = 120;

    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    uart_driver_install(UART_NUM_2, NMEA_BUF_SIZE * 2, 0, 0, NULL, 0);

    GpsHandler::ProgramUBX();

    while (1) {
        char *line = readLine(UART_NUM_2);
        //printf("%s", line);
        GpsHandler::ParseNMEA(line, pvParameters, &then);
    }
}

GpsHandler::GpsHandler(Database* db) {
    ESP_LOGI(CONFIG_SN, "[GPS] Service Stated...");

    this->db = db;
    xTaskCreate(GpsHandler::GpsChannel, "GpsChannel", 4096, db, 1, NULL);
}