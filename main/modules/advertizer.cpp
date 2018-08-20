#include "advertiser.h"

int Advertiser::Init() {
    esp_err_t err = mdns_init();
    if (err) {
        ESP_LOGI(CONFIG_SN, "[mDNS] Setup failed!");
        return 0;
    }

    mdns_hostname_set("khronos");
    mdns_instance_name_set("Khronos Device");
    return 1;
}
