#ifndef WEB_SOCKETS_H
#define WEB_SOCKETS_H

#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "lwip/api.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "esp_log.h"
#include "cJSON.h"

#include "websocket_server.h"

class WebSockets {
    public:
    int Init();
};

#endif