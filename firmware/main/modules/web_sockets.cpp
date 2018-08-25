#include "web_sockets.h"

static QueueHandle_t client_queue;
const static int client_queue_size = 10;
const static char http_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: application/json\r\n\r\n";

void WebSockets::AddKey(char* key, char* zone, char* value, cJSON* dest) {
    cJSON *key_changes = cJSON_CreateObject();

    cJSON_AddStringToObject(key_changes, "key", key);
    cJSON_AddStringToObject(key_changes, "zone", zone);
    cJSON_AddStringToObject(key_changes, "value", value);

    cJSON_AddItemToArray(dest, key_changes);
}

char* WebSockets::CompileUpdate(char* key, char* zone, Database* db) {
    cJSON *res, *changes;

	res = cJSON_CreateObject();	
	cJSON_AddItemToObject(res, "type", cJSON_CreateString("broadcast"));

    if ((zone && !zone[0]) || (key && !key[0])) {
        cJSON_AddItemToObject(res, "method", cJSON_CreateString("bulk_update"));
    } else {
        cJSON_AddItemToObject(res, "method", cJSON_CreateString("delta_update"));
    }
    
    changes = cJSON_AddArrayToObject(res, "changes");

    if (strstr(zone, "settings") || (zone && !zone[0])) {
        if (strstr(key, "ws_update_rate") || (key && !key[0])) {
            WebSockets::AddKey((char*)"ws_update_rate", (char*)"settings", (char*)std::to_string(db->GetSettings().ws_update_rate).c_str(), changes);
        }
        if (strstr(key, "led_status") || (key && !key[0])) {
            WebSockets::AddKey((char*)"led_status", (char*)"settings", (char*)std::to_string(db->GetSettings().led_status).c_str(), changes);
        }
    }
    
    if (strstr(zone, "state") || (zone && !zone[0])) {

    }

    return cJSON_Print(res);
}

char* WebSockets::BulkResponder(void* parameter, bool broadcast) {
    Database* db = (Database*)parameter;

    char* res = WebSockets::CompileUpdate((char*)"", (char*)"", db);

    if (broadcast) {
        ws_server_send_text_all_from_callback(res, strlen(res));
    }
    return res;
}

void WebSockets::DeltaResponder(char* key, char* zone, void* value) {
    char* delta = WebSockets::CompileUpdate(key, zone, ((Database*)value));
    ws_server_send_text_all_from_callback(delta, strlen(delta));
}

char* WebSockets::HandleRequest(cJSON* req, void* parameter) {
    Database* db = (Database*)parameter;
    char* res = NULL;

    if (req != NULL) {
        char* method = cJSON_GetObjectItemCaseSensitive(req, "method")->valuestring;
        char* target = cJSON_GetObjectItemCaseSensitive(req, "target")->valuestring;

        if (strstr(method, "get")) {
            if (strstr(target, "data")) {
                res = BulkResponder(db, true);
            }
        }

        if (strstr(method, "set")) {
            //int value = cJSON_GetObjectItemCaseSensitive(req, "value")->valueint;

            if (strstr(target, "led_on")) {
                Settings s = db->GetSettings();
                s.led_status = 0x0001;
                db->UpdateSettings(s);
                res = BulkResponder(db, false);
            }

            if (strstr(target, "led_off")) {
                Settings s = db->GetSettings();
                s.led_status = 0x0000;
                db->UpdateSettings(s);
                res = BulkResponder(db, false);
            }
        }
    }

    return res;
}

void WebSockets::WebSocketCallback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len, void* parameter) {
     switch(type) {
        case WEBSOCKET_CONNECT:
            ESP_LOGI(CONFIG_SN, "[SOCKET] Client #%d connected!", num);
            break;
        case WEBSOCKET_DISCONNECT_EXTERNAL:
            ESP_LOGI(CONFIG_SN, "[SOCKET] Client #%d disconnected.", num);
            break;
        case WEBSOCKET_DISCONNECT_INTERNAL:
            ESP_LOGI(CONFIG_SN, "[SOCKET] We disconnected client #%d.", num);
            break;
        case WEBSOCKET_DISCONNECT_ERROR:
            ESP_LOGI(CONFIG_SN, "[SOCKET] An error disconnected client #%d.", num);
            break;
        case WEBSOCKET_BIN:
            ESP_LOGI(CONFIG_SN, "[SOCKET] Binary Message from Client #%d: %s", num, msg);
            break;
        case WEBSOCKET_PING:
            ESP_LOGI(CONFIG_SN, "[SOCKET] Client #%d pinged us: %s", num, msg);
            break;
        case WEBSOCKET_PONG:
            ESP_LOGI(CONFIG_SN, "[SOCKET] Client #%d responded to the ping.", num);
            break;
        case WEBSOCKET_TEXT:
            ESP_LOGI(CONFIG_SN, "[SOCKET] Client #%d sent message.", num);
            break;
    }
}

void WebSockets::HttpServe(struct netconn *conn, void* parameter) {
    struct netbuf* inbuf;
    static char* buf;
    static uint16_t buflen;
    static err_t err;

    netconn_set_recvtimeout(conn, 1000);
    err = netconn_recv(conn, &inbuf);
    
    if (err == ERR_OK) {
        netbuf_data(inbuf, (void**)&buf, &buflen);

        if (buf && strstr(buf, "GET /stream ") && strstr(buf, "Upgrade: websocket")) {
            ws_server_add_client(conn, buf, buflen, (char*)"/stream", WebSockets::WebSocketCallback);
            netbuf_delete(inbuf);
        } else if (buf && strstr(buf, "POST /request ")) {
            std::string str(buf);
            str = str.substr(0, buflen);

            cJSON *req = cJSON_Parse(str.substr(str.find("\r\n\r\n")).erase(0, 4).c_str());

            if (req != NULL) {
                char* res = WebSockets::HandleRequest(req, parameter);
                netconn_write(conn, http_hdr, strlen(http_hdr), NETCONN_NOCOPY);
                netconn_write(conn, res, strlen(res), NETCONN_NOCOPY);
            }

            netconn_close(conn);
            netconn_delete(conn);
            netbuf_delete(inbuf);
        } else {
            ESP_LOGW(CONFIG_SN, "[SOCKETS] Bad request, dropping...");
            netconn_close(conn);
            netconn_delete(conn);
            netbuf_delete(inbuf);
        }
    }
}

void WebSockets::ServerHandleTask(void* pvParameters) {
    struct netconn *conn, *newconn;
    static err_t err;
    client_queue = xQueueCreate(client_queue_size, sizeof(struct netconn*));

    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 8080);
    netconn_listen(conn);
    
    ESP_LOGI(CONFIG_SN, "[SOCKETS] Server listening...");
    while (err == ERR_OK) {
        err = netconn_accept(conn, &newconn);
        if(err == ERR_OK) {
            xQueueSendToBack(client_queue, &newconn, portMAX_DELAY);
        }
    }

    ESP_LOGE(CONFIG_SN, "[SOCKETS] Task ended. Restarting...");
    netconn_close(conn);
    netconn_delete(conn);
    esp_restart();
}

void WebSockets::ServerHandleQueue(void* pvParameters) {
    struct netconn* conn;

    while (1) {
        xQueueReceive(client_queue, &conn, portMAX_DELAY);
        if(!conn) continue;
        HttpServe(conn, pvParameters);
    }
    vTaskDelete(NULL);
}

WebSockets::WebSockets(Database* db) {
    ESP_LOGI(CONFIG_SN, "[SOCKETS] Service Stated...");

    this->db = db;
    this->db->RegisterNotifier((char*)"socket", &this->DeltaResponder);

    ws_server_start(db);
    xTaskCreate(WebSockets::ServerHandleTask, "ServerHandleTask", 3000, db, 9, NULL);
    xTaskCreate(WebSockets::ServerHandleQueue, "ServerHandleQueue", 4000, db, 6, NULL);
}