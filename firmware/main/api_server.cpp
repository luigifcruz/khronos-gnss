#include "api_server.h"

static QueueHandle_t client_queue;

void ApiServer::BroadcastSerial(char* res) {
    unsigned char* msg = base64_encode((const unsigned char*)res, strlen(res), NULL);
    printf("[PAYLOAD=%s]\n", msg);
    free(msg);
}

void ApiServer::DeltaResponder(char* key, char* zone, void* value) {
    char* delta = ApiHandler::Response(key, zone, ((Database*)value));

    if (((Database*)value)->GetSettings().serial_tx_active) {
        ApiServer::BroadcastSerial(delta);
    }
    
    ws_server_send_text_all_from_callback(delta, strlen(delta));
    free(delta);
}

void ApiServer::WebSocketCallback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len, void* parameter) {
     switch(type) {
        case WEBSOCKET_CONNECT:
            ESP_LOGI(CONFIG_SN, "[API] Client #%d connected!", num);
            break;
        case WEBSOCKET_DISCONNECT_EXTERNAL:
            ESP_LOGI(CONFIG_SN, "[API] Client #%d disconnected.", num);
            break;
        case WEBSOCKET_DISCONNECT_INTERNAL:
            ESP_LOGI(CONFIG_SN, "[API] We disconnected client #%d.", num);
            break;
        case WEBSOCKET_DISCONNECT_ERROR:
            ESP_LOGI(CONFIG_SN, "[API] An error disconnected client #%d.", num);
            break;
        case WEBSOCKET_BIN:
            ESP_LOGI(CONFIG_SN, "[API] Binary Message from Client #%d: %s", num, msg);
            break;
        case WEBSOCKET_PING:
            ESP_LOGI(CONFIG_SN, "[API] Client #%d pinged us: %s", num, msg);
            break;
        case WEBSOCKET_PONG:
            ESP_LOGI(CONFIG_SN, "[API] Client #%d responded to the ping.", num);
            break;
        case WEBSOCKET_TEXT:
            ESP_LOGI(CONFIG_SN, "[API] Client #%d sent message.", num);
            break;
    }
}

void ApiServer::HttpServe(struct netconn *conn, void* parameter) {
    struct netbuf* inbuf;
    static char* buf;
    static uint16_t buflen;
    static err_t err;

    netconn_set_recvtimeout(conn, 5000);
    err = netconn_recv(conn, &inbuf);
    
    if (err == ERR_OK) {
        netbuf_data(inbuf, (void**)&buf, &buflen);

        if (buf && strstr(buf, "GET /api/stream ") && strstr(buf, "Upgrade: websocket")) {
            ws_server_add_client(conn, buf, buflen, (char*)"/api/stream", ApiServer::WebSocketCallback);
            char* bulk = ApiHandler::Response((char*)"", (char*)"", ((Database*)parameter));
            ws_server_send_text_all_from_callback(bulk, strlen(bulk));
            netbuf_delete(inbuf);
            free(bulk);
        } else if (buf && strstr(buf, "POST /api/request ")) {
            std::string str(buf);
            str = str.substr(0, buflen);

            cJSON *req = cJSON_Parse(str.substr(str.find("\r\n\r\n")).erase(0, 4).c_str());

            if (req != NULL) {
                bool broadcast = false;
                char* res = ApiHandler::Request(req, ((Database*)parameter), &broadcast);

                netconn_write(conn, http_hdr, strlen(http_hdr), NETCONN_NOCOPY);
                netconn_write(conn, res, strlen(res), NETCONN_NOCOPY);

                if (broadcast) {
                    ws_server_send_text_all_from_callback(res, strlen(res));

                    if (((Database*)parameter)->GetSettings().serial_tx_active) {
                        ApiServer::BroadcastSerial(res);
                    }
                }
            }

            cJSON_Delete(req);
            netconn_close(conn);
            netconn_delete(conn);
            netbuf_delete(inbuf);
        } else {
            ESP_LOGW(CONFIG_SN, "[API] Bad request, dropping...");

            const char* alive = "{result: 0}";
            netconn_write(conn, http_hdr, strlen(http_hdr), NETCONN_NOCOPY);
            netconn_write(conn, alive, strlen(alive), NETCONN_NOCOPY);

            netconn_close(conn);
            netconn_delete(conn);
            netbuf_delete(inbuf);
        }
    }
}

void ApiServer::ServerHandleTask(void* pvParameters) {
    struct netconn *conn, *newconn;
    static err_t err;
    client_queue = xQueueCreate(client_queue_size, sizeof(struct netconn*));

    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 8080);
    netconn_listen(conn);
    
    ESP_LOGI(CONFIG_SN, "[API] Server listening...");
    while (err == ERR_OK) {
        err = netconn_accept(conn, &newconn);
        if(err == ERR_OK) {
            xQueueSendToBack(client_queue, &newconn, portMAX_DELAY);
        }
    }

    ESP_LOGE(CONFIG_SN, "[API] Task ended. Restarting...");
    netconn_close(conn);
    netconn_delete(conn);
    esp_restart();
}

void ApiServer::ServerHandleQueue(void* pvParameters) {
    struct netconn* conn;

    while (1) {
        xQueueReceive(client_queue, &conn, portMAX_DELAY);
        if(!conn) continue;
        HttpServe(conn, pvParameters);
    }
    vTaskDelete(NULL);
}

void ApiServer::SerialResponder(void* pvParameters) {
    // Find the fucking function that get string from terminal.
}

ApiServer::ApiServer(Database* db) {
    ESP_LOGI(CONFIG_SN, "[API] Service Stated...");

    this->db = db;
    this->db->RegisterNotifier((char*)"socket", &this->DeltaResponder);

    ws_server_start(db);
    xTaskCreatePinnedToCore(ApiServer::ServerHandleTask, "ServerHandleTask", 3000, db, 9, NULL, 0);
    xTaskCreatePinnedToCore(ApiServer::ServerHandleQueue, "ServerHandleQueue", 4000, db, 6, NULL, 0);
    //xTaskCreatePinnedToCore(ApiServer::SerialResponder, "SerialResponder", 4000, db, 6, NULL, 0);
}