#include "database.h"

Settings Database::GetSettings() {
    return this->settings;
};

State Database::GetState() {
    return this->state;
};

// Database Router 

void Database::UpdateSettings(Settings data) {
    if (data.ws_update_rate != this->settings.ws_update_rate) {
        if (SettingsLoaded) { storage->WriteU16("ws_update_rate", data.ws_update_rate); }
        memcpy(&this->settings.ws_update_rate, &data.ws_update_rate, sizeof(uint16_t));

        this->snf((char*)"ws_update_rate", (char*)"settings", this);
    }

    if (data.led_status != this->settings.led_status) {
        if (SettingsLoaded) { storage->WriteU16("led_status", data.led_status); }
        memcpy(&this->settings.led_status, &data.led_status, sizeof(uint16_t));

        this->nf((char*)"led_status", (char*)"settings", &this->settings.led_status);
        this->snf((char*)"led_status", (char*)"settings", this);
    }

    this->SettingsLoaded = true;
};

void Database::UpdateState(State data) {

};

// END

// Database Initialization Registry

void Database::LoadSettings() {
    Settings s;
    memset(&s, 0, sizeof(s));
    
    s.ws_update_rate = storage->ReadU16("ws_update_rate");
    s.led_status = storage->ReadU16("led_status");

    this->UpdateSettings(s);
}

void Database::LoadState() {
    
}

// END

// Socket Notifier Registry

void Database::RegisterNotifier(char* key, Notifier nf) {
    ESP_LOGI(CONFIG_SN, "[DATABASE] New notifier registered.");

    if (strstr(key, "socket")) {
        this->snf = nf;
    }

    if (strstr(key, "led")) {
        this->nf = nf;
    }
}

// END

Database::Database(KeyStorage* storage) {
    this->storage = storage;
}