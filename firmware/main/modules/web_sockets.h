#ifndef WEB_SOCKETS_H
#define WEB_SOCKETS_H

#include <string>
#include <iostream>

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

    static void ServerHandleQueue(void* pvParameters);
    static void ServerHandleTask(void* pvParameters);
    static void HttpServe(struct netconn *conn, void* that);
    static void WebSocketCallback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len, void* parameter);
    static char* HandleRequest(cJSON* msg, void* parameter);

    static void AddKey(char* key, char* zone, char* value, cJSON* dest);
    static char* BulkResponder(void* parameter, bool broadcast);
    static char* CompileUpdate(char* key, char* zone, Database* db);
    
    public:
    WebSockets(Database* db);
    static void DeltaResponder(char* key, char* zone, void* value);
};

#endif