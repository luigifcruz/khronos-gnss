#include "ntp_server.h"

extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");

void NtpServer::InitCoprocessor() {
    esp_err_t err = ulp_load_binary(0, ulp_main_bin_start, (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t));
    if (err) {
        ESP_LOGE(CONFIG_SN, "[ULP] Failed to load coprocessor firmware.");
        return;
    }

    rtc_gpio_init(GPIO_NUM_4);
    rtc_gpio_set_direction(GPIO_NUM_4, RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_pulldown_dis(GPIO_NUM_4);
    rtc_gpio_pullup_dis(GPIO_NUM_4);
    rtc_gpio_hold_en(GPIO_NUM_4);

    rtc_gpio_init(GPIO_NUM_13);
    rtc_gpio_set_direction(GPIO_NUM_13, RTC_GPIO_MODE_OUTPUT_ONLY);

    gpio_pad_select_gpio(GPIO_NUM_12);
    gpio_set_direction(GPIO_NUM_12, GPIO_MODE_OUTPUT);

    ulp_checkmark = 0;

    ulp_set_wakeup_period(0, 0);

    err = ulp_run(&ulp_entry - RTC_SLOW_MEM);
    if (err) {
        ESP_LOGE(CONFIG_SN, "[ULP] Coprocessor failed to start!"); 
    } else {
        vTaskDelay(100 / portTICK_RATE_MS);
        ESP_LOGI(CONFIG_SN, "[ULP] Coprocessor successfully started! (Checkmark: %d)", (ulp_checkmark & UINT16_MAX)); 
    }
}

NtpServer::NtpServer() {
    this->InitCoprocessor();
}

