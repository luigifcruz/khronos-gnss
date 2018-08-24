#include "web_sockets.h"

static QueueHandle_t client_queue;
const static int client_queue_size = 10;

char* ConvertToString(uint16_t dat) {
    static char *p,buf[10];
    sprintf(buf, "%d", (int)dat);
    p = buf;
    return p;
}

void WebSockets::AddUpdate(char* key, char* zone, char* value, cJSON* dest) {
    cJSON *key_changes = cJSON_CreateObject();

    cJSON_AddStringToObject(key_changes, "key", key);
    cJSON_AddStringToObject(key_changes, "zone", zone);
    cJSON_AddStringToObject(key_changes, "value", value);

    cJSON_AddItemToArray(dest, key_changes);
}

char* WebSockets::Notifier(char* key, char* zone, char* method, void* parameter) {
    Database* db = (Database*)parameter;
    cJSON *broadcast, *changes;

	broadcast = cJSON_CreateObject();	
	cJSON_AddItemToObject(broadcast, "type", cJSON_CreateString("broadcast"));
    cJSON_AddItemToObject(broadcast, "method", cJSON_CreateString("delta_update"));

    changes = cJSON_AddArrayToObject(broadcast, "changes");

    if (strstr(zone, "settings") || (zone && !zone[0])) {
        if (strstr(key, "ws_update_rate") || (key && !key[0])) {
            WebSockets::AddUpdate("ws_update_rate", "settings", ConvertToString(db->GetSettings().ws_update_rate), changes);
        }
        if (strstr(key, "led_status") || (key && !key[0])) {
            WebSockets::AddUpdate("led_status", "settings", ConvertToString(db->GetSettings().led_status), changes);
        }
    }
    
    if (strstr(zone, "state") || (zone && !zone[0])) {

    }

    char* res = cJSON_Print(broadcast);
    if (strstr(method, "broadcast")) {
        ws_server_send_text_all(res, strlen(res));
    }
    return res;
}

char* WebSockets::HandleRequest(uint8_t num,  char* msg, uint64_t len, Database* db) {
    char* res = "ok";

    cJSON *req = cJSON_Parse(msg);
    if (req != NULL) {
        char* method = cJSON_GetObjectItemCaseSensitive(req, "method")->valuestring;

        if (strstr(method, "get_settings")) {
            res = Notifier("", "settings", "callback", db);
        }
        if (strstr(method, "get_state")) {
            res = Notifier("", "state", "callback", db);
        }

        ws_server_send_text_client_from_callback(num, res, strlen(res)); 

        if (strstr(method, "set_led")) {
            Settings s = db->GetSettings();
            s.led_status = 2000;
            db->UpdateSettings(s);
        }
    }
    
    return res;
}

void WebSockets::Broadcast(char* msg) {
    ws_server_send_text_all(msg, strlen(msg));
}

void WebSockets::WebSocketCallback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len, void* parameter) {
    Database* db = (Database*)parameter;

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
            WebSockets::HandleRequest(num, msg, len, db);
            break;
    }
}

void WebSockets::HttpServe(struct netconn *conn, WebSockets* that) {
    struct netbuf* inbuf;
    static char* buf;
    static uint16_t buflen;
    static err_t err;

    netconn_set_recvtimeout(conn, 1000);
    err = netconn_recv(conn, &inbuf);
    
    if (err == ERR_OK) {
        netbuf_data(inbuf, (void**)&buf, &buflen);

        if (buf && strstr(buf,"GET / ") && strstr(buf,"Upgrade: websocket")) {
            ws_server_add_client(conn, buf, buflen, "/", that->WebSocketCallback);
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
        HttpServe(conn, ((WebSockets*)pvParameters));
    }
    vTaskDelete(NULL);
}

WebSockets::WebSockets(Database* db) {
    ESP_LOGI(CONFIG_SN, "[SOCKETS] Service Stated...");

    this->db = db;
    this->db->RegisterSocketNotifier(&this->Notifier);    

    ws_server_start(db);
    xTaskCreate(WebSockets::ServerHandleTask, "ServerHandleTask", 3000, this, 9, NULL);
    xTaskCreate(WebSockets::ServerHandleQueue, "ServerHandleQueue", 4000, this, 6, NULL);
}