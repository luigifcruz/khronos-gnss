#include "ntp_server.h"

extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[] asm("_binary_ulp_main_bin_end");

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
        ESP_LOGI(CONFIG_SN, "[ULP] Coprocessor successfully started! (Heartbeat: %d)", (ulp_checkmark & UINT16_MAX)); 
    }
}

void NtpServer::GetTime(tstamp* time) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    time->seconds = tv.tv_sec + 2208988800UL;
    time->fraction = (uint32_t)((double)(tv.tv_usec+1) * (double)(1LL<<32) * 1.0e-6);
}

void NtpServer::UdpHandler(void* pvParameters) {
    struct sockaddr_in si_other;
    unsigned int recv_len, slen = sizeof(si_other);;
    
    int mysocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (mysocket < 0) {
        ESP_LOGE(CONFIG_SN, "[NTP SERVER] Service Error! Couldn't create socket.");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(NTPPORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);


    if (bind(mysocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(CONFIG_SN, "[NTP SERVER] Couldn't bind server.");
        close(mysocket);
        return;
    }

    ESP_LOGI(CONFIG_SN, "[NTP SERVER] Server listening...");
    char *buf = (char*)malloc(BUFFLEN);
    NtpHandler *ntp = (NtpHandler*)pvParameters;
    tstamp recv_time;

    while (1)  {
        memset(buf, 0, BUFFLEN);
            
        if ((recv_len = recvfrom(mysocket, buf, BUFFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1) {
            ESP_LOGE(CONFIG_SN, "[NTP SERVER] Couldn't receive data from client.");
            break;
        }

        NtpServer::GetTime(&recv_time);

        int err = ntp->FromBinary(buf, recv_len);
        if (err) {
            ESP_LOGE(CONFIG_SN, "[NTP SERVER] Error parsing NTP request.");
        }

        sendto(mysocket, ntp->Reply(recv_time, &NtpServer::GetTime), sizeof(NtpPacket), 0, (struct sockaddr *)&si_other, slen);
        
        ESP_LOGI(CONFIG_SN, "[NTP SERVER] Replied to NTP request from %s:%d", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        if ((recv_len + 1) < BUFFLEN) { buf[recv_len + 1] = '\0'; }
    }
        
    close(mysocket);
}

NtpServer::NtpServer() {
    ESP_LOGI(CONFIG_SN, "[NTP SERVER] Service Stated...");

    static NtpHandler ntp;

    this->InitCoprocessor();
    xTaskCreatePinnedToCore(NtpServer::UdpHandler, "UdpHandler", 2*4096, &ntp, 1, NULL, 1); 
}

