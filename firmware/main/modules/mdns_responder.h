#ifndef MDNS_H
#define MDNS_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "esp_log.h"
#include "mdns.h"

class MdnsResponder {
    public:
    int Init();
};

#endif