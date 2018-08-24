#include "web_sockets.h"

static QueueHandle_t client_queue;
const static int client_queue_size = 10;

void WebSockets::Notifier(char* key, char* zone, char* value) {
    cJSON *broadcast, *changes;

	broadcast = cJSON_CreateObject();	
	cJSON_AddItemToObject(broadcast, "type", cJSON_CreateString("broadcast"));
    cJSON_AddItemToObject(broadcast, "method", cJSON_CreateString("delta_update"));

    changes = cJSON_AddArrayToObject(broadcast, "changes");

    const char* settings_keys[] = {"ws_update_rate"};
    const char* state_keys[] = {};

    if (strstr(zone, "settings")) {
        for (int i=0; i < (sizeof(settings_keys) / sizeof(settings_keys[0])); i++) {
            if (strstr((const char*)key, settings_keys[i])) {
                cJSON *key_changes = cJSON_CreateObject();

                cJSON_AddStringToObject(key_changes, "zone", "settings");
                cJSON_AddStringToObject(key_changes, "key", key);
                cJSON_AddStringToObject(key_changes, "value", value);

                cJSON_AddItemToArray(changes, key_changes);
            }
        }
    }
    
    if (strstr(zone, "state")) {
        for (int i=0; i < (sizeof(state_keys) / sizeof(state_keys[0])); i++) {
            if (strstr((const char*)key, state_keys[i])) {
                cJSON *key_changes = cJSON_CreateObject();

                cJSON_AddStringToObject(key_changes, "zone", "state");
                cJSON_AddStringToObject(key_changes, "key", key);
                cJSON_AddStringToObject(key_changes, "value", value);

                cJSON_AddItemToArray(changes, key_changes);
            }
        }
    }

    char* res = cJSON_Print(broadcast);
    ws_server_send_text_all(res, strlen(res));
}

char* WebSockets::HandleRequest(uint8_t num,  char* msg, uint64_t len, Database* db) {
    printf("Function Called (%d).\n", (int)db);
    printf("VALUE ISSS: %d\n", db->GetSettings().ws_update_rate);
    return "yomamma";
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
            char* res = WebSockets::HandleRequest(num, msg, len, db);
            ws_server_send_text_client_from_callback(num, res, strlen(res));
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