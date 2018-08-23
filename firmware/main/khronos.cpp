#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_log.h"

#include "modules/key_storage.h"
#include "modules/flash_storage.h"
#include "modules/wireless.h"
#include "modules/http_server.h"
#include "modules/mdns_responder.h"
#include "modules/web_sockets.h"
#include "modules/database.h"

extern "C" {
    void app_main();
}

void app_main() {
    KeyStorage keys;
    Wireless wifi;
    HttpServer web;
    FlashStorage flash;
    MdnsResponder mdns;
    WebSockets ws;
    static Database db;
    
    keys.Init();
    db.Init(&keys);

    flash.Init();
    mdns.Init();
    wifi.Init();
    
    web.Init();
    ws.Init(&db);

    printf("Trying to read the value!\n");
    uint32_t read = keys.ReadU32("ws_update_rate");
    printf("Operation exited with %d\n", read);
}