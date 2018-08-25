#include "esp_system.h"
#include "esp_log.h"

#include "modules/key_storage.h"
#include "modules/flash_storage.h"
#include "modules/wireless.h"
#include "modules/http_server.h"
#include "modules/mdns_responder.h"
#include "modules/api_server.h"
#include "modules/database.h"
#include "driver/gpio.h"

extern "C" {
    void app_main();
}

static void LedNotifier(char* key, char* zone, void* value) {
    uint16_t* lvl = (uint16_t*)value;
    gpio_set_level(GPIO_NUM_2, (uint32_t)*lvl);
}

void app_main() {
    static KeyStorage keys;
    static Database db(&keys);

    FlashStorage flash;
    MdnsResponder mdns;
    Wireless wifi;
    
    HttpServer web;
    ApiServer ws(&db);

    db.RegisterNotifier((char*)"led", LedNotifier);

    gpio_pad_select_gpio(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

    ESP_LOGI(CONFIG_SN, "[MAIN] Loading state and settings...");
    db.LoadSettings();
    db.LoadState();

/*
    uint32_t up = 200;
    while(1) {
        Settings s = db.GetSettings();
        s.ws_update_rate = up++;
        db.UpdateSettings(s);

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
*/
}