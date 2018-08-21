#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_log.h"

#include "flash_storage.h"

#include <cstring>
#include <sys/param.h>
#include <http_server.h>

class HttpServer {
    private:
    httpd_handle_t server = NULL;

    public:
    int Init();
    void Stop();
};

#endif