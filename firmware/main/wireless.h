#ifndef WIRELESS_H
#define WIRELESS_H

#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "mdns.h"

class Wireless {
    private:
    static esp_err_t EventHandler(void *ctx, system_event_t *event);

    public:
    Wireless();
};

#endif