#ifndef FLASHSTORAGE_H
#define FLASHSTORAGE_H

#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_spiffs.h"

#include <stdio.h>
#include <string.h>

class FlashStorage {
    public:
    FlashStorage();
    void Close();
};

#endif