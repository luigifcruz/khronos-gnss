#include "flash_storage.h"

void FlashStorage::Close() {
    ESP_LOGE(CONFIG_SN, "[FLASH_FS] Closing service...");
    esp_vfs_spiffs_unregister(NULL);
}

FlashStorage::FlashStorage() {
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = false
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(CONFIG_SN, "[FLASH_FS] Failed to mount or format filesystem.");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(CONFIG_SN, "[FLASH_FS] Failed to find SPIFFS partition.");
        } else {
            ESP_LOGE(CONFIG_SN, "[FLASH_FS] Failed to initialize SPIFFS (%s).", esp_err_to_name(ret));
        }
    }
    
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(CONFIG_SN, "[FLASH_FS] Failed to get SPIFFS partition information (%s).", esp_err_to_name(ret));
    } else {
        ESP_LOGI(CONFIG_SN, "[FLASH_FS] Partition size: total: %d, used: %d.", total, used);
    }
}