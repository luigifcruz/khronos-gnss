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
#include "database.h"

class WebSockets {
    private:
    Database* db;

    static void ServerHandleQueue(void* taskStartParameters);
    static void ServerHandleTask(void* pvParameters);
    static void HttpServe(struct netconn *conn, void* task);
    static void WebSocketCallback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len, void* parameter);

    public:
    int Init(Database* db);
};

#endif