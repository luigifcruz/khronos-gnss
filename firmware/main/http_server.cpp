#include "http_server.h"

void serve(char *filename, httpd_req_t *req)
{
    if (strstr(filename, ".css.gz"))
    {
        httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
        httpd_resp_set_type(req, "text/css");
    }
    else if (strstr(filename, ".js.gz"))
    {
        httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
        httpd_resp_set_type(req, "text/javascript");
    }
    else if (strstr(filename, ".jpeg"))
    {
        httpd_resp_set_type(req, "image/jpeg");
    }
    else if (strstr(filename, ".png"))
    {
        httpd_resp_set_type(req, "image/png");
    }
    else
    {
        httpd_resp_set_type(req, "text/html");
    }

    FILE *file = NULL;
    char file_buffer[BUF_SIZE];
    unsigned int file_size = 0, bytes_read = 0;
    file = fopen(filename, "r");

    if (file == NULL)
    {
        httpd_resp_send_404(req);
        return;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char cont_len[8];
    sprintf(cont_len, "%d", file_size);
    httpd_resp_set_hdr(req, "content-length", cont_len);

    while (bytes_read < file_size)
    {
        if (fread(file_buffer, BUF_SIZE, 1, file) == 0)
        {
            httpd_resp_send_chunk(req, file_buffer, file_size % BUF_SIZE);
        }
        else
        {
            httpd_resp_send_chunk(req, file_buffer, (file_size > BUF_SIZE) ? BUF_SIZE : file_size);
        }
        bytes_read += BUF_SIZE;
    }

    httpd_resp_send_chunk(req, file_buffer, 0);

    fclose(file);
}

esp_err_t file_server_handler(httpd_req_t *req)
{
    serve((char *)req->uri, req);
    return ESP_OK;
}

esp_err_t react_server_handler(httpd_req_t *req)
{
    serve("/spiffs/index.html", req);
    return ESP_OK;
}

void HttpServer::Stop()
{
    httpd_stop(this->server);
}

HttpServer::HttpServer()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(CONFIG_SN, "[HTTP] Service stated on port %d!", config.server_port);

    if (httpd_start(&this->server, &config) == ESP_OK)
    {
        httpd_uri_t react = {
            .uri = "/*",
            .method = HTTP_GET,
            .handler = react_server_handler,
            .user_ctx = &server};

        httpd_uri_t file_server = {
            .uri = "/spiffs/*",
            .method = HTTP_GET,
            .handler = file_server_handler,
            .user_ctx = &server};

        //httpd_register_uri_handler(server, &react);
        httpd_register_uri_handler(server, &file_server);
    }
    else
    {
        ESP_LOGI(CONFIG_SN, "[HTTP] Error starting HTTP server!");
    }
}