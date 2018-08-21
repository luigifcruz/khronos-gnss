#ifndef KEYSTORAGE_H
#define KEYSTORAGE_H

#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"

class KeyStorage {
    public:
    int Init();
};

#endif