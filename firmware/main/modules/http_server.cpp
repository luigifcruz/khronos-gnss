#include "http_server.h"

#define BUF_SIZE 1024

esp_err_t get_handler(httpd_req_t *req) {
    FILE *file = NULL;
    char file_buffer[BUF_SIZE];
    unsigned int file_size = 0;
    unsigned int bytes_read = 0;
    char *file_name = (char*)req->user_ctx;
    
    if (file_name == NULL) {
        char *query = (char *)malloc(64);
        char *file_n = (char *)malloc(64);

        httpd_req_get_url_query_str(req, query, 64);

        strcpy(file_n, "/spiffs/cdn/");
        strcat(file_n, query);

        file = fopen(file_n, "r");
        httpd_resp_set_type(req, "application/octet-stream");
    } else {
        file = fopen(file_name, "r");
        httpd_resp_set_type(req, "text/html");
    }

    if(file == NULL) {
        httpd_resp_send_404(req);
        return ESP_OK;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    while (bytes_read < file_size) {
        fread(file_buffer, BUF_SIZE, 1, file);
        httpd_resp_send_chunk(req, file_buffer, (file_size > BUF_SIZE) ? BUF_SIZE : file_size);
        bytes_read += BUF_SIZE;
    }

    httpd_resp_send_chunk(req, file_buffer, 0);
    
    fclose(file);
    return ESP_OK;
}

httpd_uri_t gnss = {
    .uri       = "/gnss",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/gnss.html"
};

httpd_uri_t clock = {
    .uri       = "/clock",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/clock.html"
};

httpd_uri_t settings = {
    .uri       = "/settings",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/settings.html"
};

httpd_uri_t tuner = {
    .uri       = "/tuner",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/tuner.html"
};

httpd_uri_t dashboard = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/dashboard.html"
};

httpd_uri_t file = {
    .uri       = "/file",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = NULL
};

void HttpServer::Stop() {
    httpd_stop(this->server);
}

HttpServer::HttpServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(CONFIG_SN, "[HTTP] Service stated on port %d!", config.server_port);

    if (httpd_start(&this->server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &dashboard);
        httpd_register_uri_handler(server, &tuner);
        httpd_register_uri_handler(server, &settings);
        httpd_register_uri_handler(server, &clock);
        httpd_register_uri_handler(server, &gnss);
        httpd_register_uri_handler(server, &file);
    } else {
        ESP_LOGI(CONFIG_SN, "[HTTP] Error starting HTTP server!");
    }
}