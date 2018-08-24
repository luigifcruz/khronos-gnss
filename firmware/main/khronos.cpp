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
    static Database db(&keys);

    FlashStorage flash;
    MdnsResponder mdns;
    Wireless wifi;
    
    HttpServer web;
    WebSockets ws(&db);

    uint32_t up = 200;
    while(1) {
        Settings s = db.GetSettings();
        s.ws_update_rate = up++;
        db.UpdateSettings(s);

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}