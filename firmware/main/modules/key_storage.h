#ifndef KEYSTORAGE_H
#define KEYSTORAGE_H

#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"

class KeyStorage {
    private:
    nvs_handle handle;
    esp_err_t ret;

    int CheckError();

    public:
    KeyStorage();

    int WriteU32(const char* key, uint32_t num);
    uint32_t ReadU32(const char* key);

    int WriteU16(const char* key, uint16_t num);
    uint16_t ReadU16(const char* key);

    int Commit();
};

#endif