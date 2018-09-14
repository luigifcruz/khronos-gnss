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
        ESP_LOGI(CONFIG_SN, "[ULP] Coprocessor successfully started! (Checkmark: %d)", (ulp_checkmark & UINT16_MAX)); 
    }
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
    char buf[BUFFLEN];

    while (1)  {
        memset(buf, 0, BUFFLEN);
            
        if ((recv_len = recvfrom(mysocket, buf, BUFFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1) {
            ESP_LOGE(CONFIG_SN, "[NTP SERVER] Couldn't receive data from client.");
            break;
        }
            
        ESP_LOGI(CONFIG_SN,"Received packet from %s:%d", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

        sendto(mysocket, buf, strlen(buf), 0, (struct sockaddr *)&si_other, slen);

        int i,j;
        for (j = 0; j < recv_len; j++) {
            for (i = 0; i < 8; i++) {
                printf("%d", !!((buf[j] << i) & 0x80));
            }
        }
        printf("\n");

        NtpPacket ntp;
        memcpy(&ntp, buf, sizeof(ntp));

        printf("Peer Clock Precision: %d\n", ntp.precision);
        printf("Root Delay: %u\n", ntohl(ntp.rootdelay));
        printf("Root Dispersion: %u\n", ntp.rootdisp);
            
        if ((recv_len + 1) < BUFFLEN) { buf[recv_len + 1] = '\0'; }
    }
        
    close(mysocket);
}

NtpServer::NtpServer() {
    ESP_LOGI(CONFIG_SN, "[NTP SERVER] Service Stated...");

    this->InitCoprocessor();
    xTaskCreate(NtpServer::UdpHandler, "UdpHandler", 2*4096, NULL, 20, NULL); 
}

