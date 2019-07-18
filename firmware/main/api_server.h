#ifndef API_SERVER_H
#define API_SERVER_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "lwip/api.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "esp_log.h"
#include "cJSON.h"
#include "api_handler.h"
#include "websocket_server.h"
#include "database.h"

extern "C" {
#include "wpa2/utils/base64.h"
};

const static int client_queue_size = 5;
const static char http_hdr[] = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-type: application/json\r\nConnection: close\r\n\r\n";

class ApiServer {
    private:
    Database* db;

    static void ServerHandleQueue(void* pvParameters);
    static void ServerHandleTask(void* pvParameters);
    static void HttpServe(struct netconn *conn, void* that);
    static void WebSocketCallback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len, void* parameter);
    static void BroadcastSerial(char* res);
    static void SerialResponder(void* pvParameters);
    
    public:
    ApiServer(Database* db);
    static void DeltaResponder(char* key, char* zone, void* value);
};

#endif