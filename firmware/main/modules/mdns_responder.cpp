#include "mdns_responder.h"

MdnsResponder::MdnsResponder() {
    ESP_LOGI(CONFIG_SN, "[MDNS] Service Stated...");

    esp_err_t err = mdns_init();
    if (err) {
        ESP_LOGE(CONFIG_SN, "[MDNS] Init failed!");
    }

    mdns_hostname_set(CONFIG_MDNS_HOSTNAME);
    mdns_instance_name_set(CONFIG_MDNS_INSTANCE);

    mdns_service_add("Web Server", "_http", "_tcp", 80, NULL, 0);
}