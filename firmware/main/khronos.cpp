#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_log.h"

#include "modules/key_storage.h"
#include "modules/flash_storage.h"
#include "modules/wireless.h"
#include "modules/http_server.h"

extern "C" {
    void app_main();
}

void app_main() {
    KeyStorage keys;
    Wireless wifi;
    HttpServer web;
    FlashStorage flash;

    keys.Init();
    wifi.Init();
    web.Init();
    flash.Init();
}