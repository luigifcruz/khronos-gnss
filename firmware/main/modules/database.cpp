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

    if (data.serial_tx_active != this->settings.serial_tx_active) {
        if (SettingsLoaded) { storage->WriteU8("se_tx_active", data.serial_tx_active); }
        memcpy(&this->settings.serial_tx_active, &data.serial_tx_active, sizeof(uint8_t));

        this->snf((char*)"serial_tx_active", (char*)"settings", this);
    }
};

void Database::UpdateState(State data) {
    if (data.gps_fix_quality != this->state.gps_fix_quality) {
        memcpy(&this->state.gps_fix_quality, &data.gps_fix_quality, sizeof(uint8_t));

        this->snf((char*)"gps_fix_quality", (char*)"state", this);
    }

    if (data.gps_fix_type != this->state.gps_fix_type) {
        memcpy(&this->state.gps_fix_type, &data.gps_fix_type, sizeof(uint8_t));

        this->snf((char*)"gps_fix_type", (char*)"state", this);
    }

    if (data.gps_sat_numb != this->state.gps_sat_numb) {
        memcpy(&this->state.gps_sat_numb, &data.gps_sat_numb, sizeof(uint8_t));

        this->snf((char*)"gps_sat_numb", (char*)"state", this);
    }

    if (data.glonass_sat_numb != this->state.glonass_sat_numb) {
        memcpy(&this->state.glonass_sat_numb, &data.glonass_sat_numb, sizeof(uint8_t));

        this->snf((char*)"glonass_sat_numb", (char*)"state", this);
    }
};

// END

// Database Initialization Registry

void Database::LoadSettings() {
    Settings s;
    memset(&s, 0, sizeof(s));
    
    s.ws_update_rate = storage->ReadU16("ws_update_rate");
    s.led_status = storage->ReadU16("led_status");
    s.serial_tx_active = storage->ReadU8("se_tx_active");

    this->UpdateSettings(s);
    this->SettingsLoaded = true;
}

void Database::LoadState() {
    State s;
    memset(&s, 0, sizeof(s));

    s.gps_fix_quality = 0;
    s.gps_fix_type = 0;
    s.gps_sat_numb = 0;
    s.glonass_sat_numb = 0;

    this->UpdateState(s);
    this->StateLoaded = true;
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