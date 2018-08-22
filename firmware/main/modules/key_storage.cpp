#include "key_storage.h"

int KeyStorage::CheckError() {
    if (this->ret != ESP_OK) {
        ESP_LOGE(CONFIG_SN, "[NVS] Operation error!");
        return 1;
    }
    return 0;
}

int KeyStorage::WriteU32(const char* key, uint32_t num) {
    this->ret = nvs_set_u32(this->handle, key, num);
    return this->Commit() || CheckError();
}

uint32_t KeyStorage::ReadU32(const char* key) {
    uint32_t buf = 0;
    this->ret = nvs_get_u32(this->handle, key, &buf);
    CheckError();
    return buf;
}

int KeyStorage::Commit() {
    this->ret = nvs_commit(this->handle);
    if (this->ret != ESP_OK) {
        ESP_LOGE(CONFIG_SN, "[NVS] Commit failed!");
        return 1;
    }
    return 0;
}

int KeyStorage::Init() {
    ESP_LOGI(CONFIG_SN, "[NVS] Service Stated...");

    this->ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    this->ret = nvs_open("storage", NVS_READWRITE, &this->handle);
    if (this->ret != ESP_OK) {
        ESP_LOGE(CONFIG_SN, "[NVS] Error opening NVS handle!");
        return 1;
    }
    
    return 0;
}