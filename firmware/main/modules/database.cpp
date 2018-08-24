#include "database.h"

void Database::RegisterSocketNotifier(Notifier snf) {
    ESP_LOGI(CONFIG_SN, "[DATABASE] WebSocket Notifier Registered!");
    this->snf = snf;
}

Settings Database::GetSettings() {
    return this->settings;
};

State Database::GetState() {
    return this->state;
};

void Database::UpdateSettings(Settings data) {
    if (data.ws_update_rate != this->settings.ws_update_rate) {
        storage->WriteU32("ws_update_rate", data.ws_update_rate);
        memcpy(&this->settings.ws_update_rate, &data.ws_update_rate, sizeof(Settings));
        this->snf("ws_update_rate", "settings", "broadcast", this);
    }

    if (data.led_status != this->settings.led_status) {
        storage->WriteU32("led_status", data.led_status);
        memcpy(&this->settings.led_status, &data.led_status, sizeof(Settings));
        this->snf("led_status", "settings", "broadcast", this);
    }
};

void Database::UpdateState(State data) {

};

void Database::LoadSettings() {
    this->settings.ws_update_rate = storage->ReadU32("ws_update_rate");
    this->settings.ws_update_rate = storage->ReadU32("led_status");
}

void Database::LoadState() {
}

Database::Database(KeyStorage* storage) {
    this->storage = storage;

    LoadSettings();
    LoadState();
}