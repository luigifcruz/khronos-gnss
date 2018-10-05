#include "display.h"

void Display::Clear() {
    i2c_cmd_handle_t cmd;

    for (uint8_t i = 0; i < 8; i++) {
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (DISLAY_ADDR << 1) | I2C_MASTER_WRITE, true);

        i2c_master_write_byte(cmd, 0x80, true);
        i2c_master_write_byte(cmd, 0xB0 | i, true);

        i2c_master_write_byte(cmd, 0x40, true);
        for (uint8_t j = 0; j < 128; j++) {
            i2c_master_write_byte(cmd, 0x00, true);
        }
        
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);
    }
}

void Display::Text(char* text) {
    uint8_t text_len = strlen(text);

    i2c_cmd_handle_t cmd;

    uint8_t cur_page = 0;

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (DISLAY_ADDR << 1) | I2C_MASTER_WRITE, true);

    i2c_master_write_byte(cmd, 0x00, true);
    i2c_master_write_byte(cmd, 0x00, true); // reset column
    i2c_master_write_byte(cmd, 0x10, true);
    i2c_master_write_byte(cmd, 0xB0 | cur_page, true); // reset page

    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    for (uint8_t i = 0; i < text_len; i++) {
        if (text[i] == '\n') {
            cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (DISLAY_ADDR << 1) | I2C_MASTER_WRITE, true);

            i2c_master_write_byte(cmd, 0x00, true);
            i2c_master_write_byte(cmd, 0x00, true); // reset column
            i2c_master_write_byte(cmd, 0x10, true);
            i2c_master_write_byte(cmd, 0xB0 | ++cur_page, true); // increment page

            i2c_master_stop(cmd);
            i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd);
        } else {
            cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (DISLAY_ADDR << 1) | I2C_MASTER_WRITE, true);

            i2c_master_write_byte(cmd, 0x40, true);
            i2c_master_write(cmd, (uint8_t*)font8x8_basic_tr[(uint8_t)text[i]], 8, true);

            i2c_master_stop(cmd);
            i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd);
        }
    }
}

void Display::Engine(void* parameter) {
    time_t now;
    struct tm tm;
    char buf[256];
    char fix[9];

    while(1) {
        State s = ((Database*)parameter)->GetState();
        
        time(&now);
        localtime_r(&now, &tm);

        if (s.gnss_fix_quality == 0) {
            sprintf(fix, "%02d NO FIX", s.sat_count_gps + s.sat_count_glonass + s.sat_count_galileo);
        } else if (s.gnss_fix_quality == 1) {
            sprintf(fix, "  %02d GNSS", s.sat_count_gps + s.sat_count_glonass + s.sat_count_galileo);
        } else if (s.gnss_fix_quality == 2) {
            sprintf(fix, " %02d DGNSS", s.sat_count_gps + s.sat_count_glonass + s.sat_count_galileo);
        } else if (s.gnss_fix_quality == 6) {
            sprintf(fix, "%02d DRGNSS", s.sat_count_gps + s.sat_count_glonass + s.sat_count_galileo);
        }

        sprintf(buf, "Lat: %011.6f\nLon: %011.6f\nAlt(m):   %06.1f\nSpeed:    %06.2f\nSats:  %s\nTime:  %02d:%02d:%02dZ\nDate: %02d/%02d/%04d",
            s.latitude, s.longitude, s.altitude, s.ground_speed, fix,
            tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_mday, tm.tm_mon, tm.tm_year + 1900);
        Display::Text((char*)&buf);

        vTaskDelay(50/portTICK_PERIOD_MS);
    }
}

Display::Display(Database* db) {  
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)SDA_PIN;
    conf.scl_io_num = (gpio_num_t)SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 1000000;
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

    this->db = db;
    ESP_LOGI(CONFIG_SN, "[DISPLAY] Service Stated...");

    i2c_cmd_handle_t port = i2c_cmd_link_create();

    i2c_master_start(port);
    i2c_master_write_byte(port, (DISLAY_ADDR << 1) | I2C_MASTER_WRITE, true);

    i2c_master_write_byte(port, 0xAE, true);
    i2c_master_write_byte(port, 0xD3, true);
    i2c_master_write_byte(port, 0x00, true);
    i2c_master_write_byte(port, 0x40 | 0x00, true);
    i2c_master_write_byte(port, 0x8D, true);
    i2c_master_write_byte(port, 0x14, true);
    i2c_master_write_byte(port, 0x20, true);
    i2c_master_write_byte(port, 0x00, true);
    i2c_master_write_byte(port, 0xA0 | 0x10, true);
    i2c_master_write_byte(port, 0xC8, true);
    i2c_master_write_byte(port, 0xDA, true);
    i2c_master_write_byte(port, 0x12, true);
    i2c_master_write_byte(port, 0x81, true);
    i2c_master_write_byte(port, 0xCF, true);
    i2c_master_write_byte(port, 0xd9, true);
    i2c_master_write_byte(port, 0xF1, true);
    i2c_master_write_byte(port, 0xDB, true);
    i2c_master_write_byte(port, 0x40, true);
    i2c_master_write_byte(port, 0xA4, true);
    i2c_master_write_byte(port, 0xA1, true);
    i2c_master_write_byte(port, 0xC8, true);
    i2c_master_write_byte(port, 0xAF, true);
    i2c_master_stop(port);

    esp_err_t err = i2c_master_cmd_begin(I2C_NUM_0, port, 100/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(port);

    vTaskDelay(100/portTICK_PERIOD_MS);

    if (err == ESP_OK) {
        ESP_LOGI(CONFIG_SN, "[DISPLAY] Display ready...");

        Display::Clear();
        xTaskCreatePinnedToCore(Display::Engine, "DisplayEngine", 4096, db, 20, NULL, 1); 
    } else {
        ESP_LOGE(CONFIG_SN, "[DISPLAY] Display configuration errored. Code: 0x%.2X", err);
    }
}