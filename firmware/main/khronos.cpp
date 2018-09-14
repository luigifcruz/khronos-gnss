#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"

#include "modules/key_storage.h"
#include "modules/flash_storage.h"
#include "modules/wireless.h"
#include "modules/http_server.h"
#include "modules/mdns_responder.h"
#include "modules/api_server.h"
#include "modules/database.h"
#include "modules/ntp_server.h"
#include "modules/gps_handler.h"

static void LedNotifier(char* key, char* zone, void* value) {
    uint16_t* lvl = (uint16_t*)value;
    gpio_set_level(GPIO_NUM_2, (uint32_t)*lvl);
}

extern "C" {
    void app_main();
}

void app_main() {
    static KeyStorage keys;
    static Database db(&keys);

    FlashStorage flash;
    MdnsResponder mdns;
    Wireless wifi;
    
    HttpServer web;
    ApiServer ws(&db);
    NtpServer ntp;
    GpsHandler gps;
  
    db.RegisterNotifier((char*)"led", LedNotifier);

    gpio_pad_select_gpio(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

    ESP_LOGI(CONFIG_SN, "[MAIN] Loading state and settings...");
    db.LoadSettings();
    db.LoadState();
}