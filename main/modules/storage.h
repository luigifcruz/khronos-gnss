#ifndef STORAGE_H
#define STORAGE_H

#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"

class Storage {
    public:
    int Init();
};

#endif