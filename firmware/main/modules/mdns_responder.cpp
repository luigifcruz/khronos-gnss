#include "mdns_responder.h"

static const char *TAG = "mdns-test";

int MdnsResponder::Init() {
    ESP_LOGI(CONFIG_SN, "[MDNS] Service Stated...");

    esp_err_t err = mdns_init();
    if (err) {
        ESP_LOGE(CONFIG_SN, "[MDNS] Init failed!");
        return 1;
    }

    mdns_hostname_set(CONFIG_MDNS_HOSTNAME);
    mdns_instance_name_set(CONFIG_MDNS_INSTANCE);

    mdns_service_add(NULL, "_http", "_tcp", 80, NULL, 0);

    ESP_LOGI(CONFIG_SN, "[MDNS] Init completed!");
    return 0;
}