#include "wireless.h"

static EventGroupHandle_t wifi_event_group = NULL;

esp_err_t Wireless::EventHandler(void *ctx, system_event_t *event) {
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        ESP_LOGI(CONFIG_SN, "[WIRELESS] Service Stated...");
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(CONFIG_SN, "[WIRELESS] Got IP: %s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        xEventGroupSetBits(wifi_event_group, BIT0);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        ESP_LOGI(CONFIG_SN, "[WIRELESS] Disconnected. Trying to reconnect...");
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, BIT0);
        break;
    default:
        break;
    }

    mdns_handle_system_event(ctx, event);
    return ESP_OK;
}

int Wireless::Init() {
    wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    esp_event_loop_init(this->EventHandler, this);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = { };
    strcpy((char*)wifi_config.sta.ssid, CONFIG_WIFI_SSID);
    strcpy((char*)wifi_config.sta.password, CONFIG_WIFI_PASSWORD);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(CONFIG_SN, "[WIRELESS] Settings SSID: %s", CONFIG_WIFI_SSID);

    return 1;
}