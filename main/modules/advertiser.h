#ifndef ADVERTISER_H
#define ADVERTISER_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "mdns.h"

class Advertiser {
    public:
    int Init();
};

#endif