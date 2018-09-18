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

void GpsHandler::ParseNMEA(char* line, void* pvParameters) {
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

        default: break;
    }

    db->UpdateState(s);
}

void GpsHandler::ProgramUBX() {
    const char ubx_enable_zda[] = {0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x5a, 0x44, 0x41, 0x2a, 0x33, 0x39, 0x0d, 0x0a, 0xb5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x08, 0x01, 0x03, 0x20};
    uart_write_bytes(UART_NUM_2, (const char *)&ubx_enable_zda, sizeof(ubx_enable_zda));
    ESP_LOGI(CONFIG_SN, "[GPS] UBX module programmed.");
}

void GpsHandler::GpsChannel(void* pvParameters) {
    uart_config_t uart_config;
    
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
        GpsHandler::ParseNMEA(line, pvParameters);
    }
}

GpsHandler::GpsHandler(Database* db) {
    ESP_LOGI(CONFIG_SN, "[GPS] Service Stated...");

    this->db = db;
    xTaskCreate(GpsHandler::GpsChannel, "GpsChannel", 4096, db, 10, NULL);
}