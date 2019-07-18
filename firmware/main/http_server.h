#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <cstring>
#include <string>
#include <sys/param.h>
#include <esp_http_server.h>

#include "flash_storage.h"
#include "esp_system.h"
#include "esp_log.h"

#define BUF_SIZE 1024

class HttpServer
{
private:
    httpd_handle_t server = NULL;

public:
    HttpServer();
    void Stop();
};

#endif