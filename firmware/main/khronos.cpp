#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_log.h"

#include "modules/key_storage.h"
#include "modules/flash_storage.h"
#include "modules/wireless.h"
#include "modules/http_server.h"
#include "modules/mdns_responder.h"
#include "modules/web_sockets.h"

extern "C" {
    void app_main();
}

void app_main() {
    KeyStorage keys;
    Wireless wifi;
    HttpServer web;
    FlashStorage flash;
    MdnsResponder mdns;
    WebSockets socks;
    
    keys.Init();
    flash.Init();

    mdns.Init();
    wifi.Init();
    
    web.Init();
    socks.Init();
}