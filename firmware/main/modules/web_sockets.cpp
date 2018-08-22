#include "web_sockets.h"

static QueueHandle_t client_queue;
const static int client_queue_size = 10;

void WebSocketCallback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len) {
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
        cJSON *root = cJSON_Parse(msg);

        int number = cJSON_GetObjectItem(root, "num")->valueint;
        cJSON_SetIntValue(cJSON_GetObjectItem(root, "num"), number+10);
        cJSON_SetIntValue(cJSON_GetObjectItem(root, "fdkshjf"), 69);
        printf("Sent number is %d. After: %d %d\n", number, cJSON_GetObjectItem(root, "num")->valueint, cJSON_GetObjectItem(root, "fdkshjf")->valueint);
        char *rendered = cJSON_Print(root);
        printf("String: %s\n", rendered);
        ws_server_send_text_client_from_callback(num, rendered, strlen(rendered));
        cJSON_Delete(root);
        break;
  }
}

static void HttpServe(struct netconn *conn) {
    struct netbuf* inbuf;
    static char* buf;
    static uint16_t buflen;
    static err_t err;

    netconn_set_recvtimeout(conn, 1000);
    err = netconn_recv(conn, &inbuf);
    
    if(err==ERR_OK) {
        netbuf_data(inbuf, (void**)&buf, &buflen);

        if (buf && strstr(buf,"GET / ") && strstr(buf,"Upgrade: websocket")) {
            ws_server_add_client(conn, buf, buflen, "/", WebSocketCallback);
            netbuf_delete(inbuf);
        } else {
            ESP_LOGW(CONFIG_SN, "[SOCKETS] Bad request, dropping...");
            netconn_close(conn);
            netconn_delete(conn);
            netbuf_delete(inbuf);
        }
    }
}

static void ServerHandleTask(void* pvParameters) {
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

static void ServerHandleQueue(void* pvParameters) {
    struct netconn* conn;
    
    while (1) {
        xQueueReceive(client_queue, &conn, portMAX_DELAY);
        if(!conn) continue;
        HttpServe(conn);
    }
    vTaskDelete(NULL);
}

int WebSockets::Init() {
    ESP_LOGI(CONFIG_SN, "[SOCKETS] Service Stated...");

    ws_server_start();
    xTaskCreate(&ServerHandleTask, "ServerHandleTask", 3000, NULL, 9, NULL);
    xTaskCreate(&ServerHandleQueue, "ServerHandleQueue", 4000, NULL, 6, NULL);
    return 0;
}