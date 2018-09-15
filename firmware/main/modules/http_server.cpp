#include "http_server.h"

esp_err_t get_handler(httpd_req_t *req) {
    FILE *file = NULL;
    char file_buffer[BUF_SIZE];
    unsigned int file_size = 0;
    unsigned int bytes_read = 0;
    char *file_name = (char*)req->user_ctx;
    
    file = fopen(file_name, "r");

    if (strstr((char*)req->uri, ".css")) {
        httpd_resp_set_type(req, "text/css");
    } else if (strstr((char*)req->uri, ".js")) {
        httpd_resp_set_type(req, "application/javascript");
    } else {
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
        if (fread(file_buffer, BUF_SIZE, 1, file) == 0) {
            httpd_resp_send_chunk(req, file_buffer, file_size % BUF_SIZE);
        } else {
            httpd_resp_send_chunk(req, file_buffer, (file_size > BUF_SIZE) ? BUF_SIZE : file_size);
        }
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
    .user_ctx  = (void*)"/spiffs/index.html"
};

httpd_uri_t clock = {
    .uri       = "/clock",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/index.html"
};

httpd_uri_t settings = {
    .uri       = "/settings",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/index.html"
};

httpd_uri_t tuner = {
    .uri       = "/tuner",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/index.html"
};

httpd_uri_t dashboard = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/index.html"
};

httpd_uri_t style = {
    .uri       = "/style.css",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/style.css"
};

httpd_uri_t bundle = {
    .uri       = "/bundle.js",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = (void*)"/spiffs/bundle.js"
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
        httpd_register_uri_handler(server, &style);
        httpd_register_uri_handler(server, &bundle);
    } else {
        ESP_LOGI(CONFIG_SN, "[HTTP] Error starting HTTP server!");
    }
}