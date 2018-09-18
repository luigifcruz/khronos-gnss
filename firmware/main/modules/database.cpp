#include "database.h"

Settings Database::GetSettings() {
    return this->settings;
};

State Database::GetState() {
    return this->state;
};

// Database Router 

void Database::UpdateSettings(Settings data) {
    std::string updates;

    if (data.led_status != this->settings.led_status) {
        if (SettingsLoaded) { storage->WriteU16("led_status", data.led_status); }
        memcpy(&this->settings.led_status, &data.led_status, sizeof(uint16_t));
        updates.append("led_status ");

        this->nf((char*)"led_status", (char*)"settings", &this->settings.led_status);
    }

    if (data.serial_tx_active != this->settings.serial_tx_active) {
        if (SettingsLoaded) { storage->WriteU8("se_tx_active", data.serial_tx_active); }
        memcpy(&this->settings.serial_tx_active, &data.serial_tx_active, sizeof(uint8_t));
        updates.append("serial_tx_active ");
    }

    if (updates.length() > 0) {
        this->snf((char*)updates.c_str(), (char*)"settings", this);
    }
};

void Database::UpdateState(State data) {
    std::string updates;

    if (data.gnss_fix_quality != this->state.gnss_fix_quality) {
        memcpy(&this->state.gnss_fix_quality, &data.gnss_fix_quality, sizeof(uint8_t));
        updates.append("gnss_fix_quality ");
    }

    if (data.gnss_fix_type != this->state.gnss_fix_type) {
        memcpy(&this->state.gnss_fix_type, &data.gnss_fix_type, sizeof(uint8_t));
        updates.append("gnss_fix_type ");
    }

    if (data.sat_count_gps != this->state.sat_count_gps) {
        memcpy(&this->state.sat_count_gps, &data.sat_count_gps, sizeof(uint8_t));
        updates.append("sat_count_gps ");
    }

    if (data.sat_count_glonass != this->state.sat_count_glonass) {
        memcpy(&this->state.sat_count_glonass, &data.sat_count_glonass, sizeof(uint8_t));
        updates.append("sat_count_glonass ");
    }

    if (data.latitude != this->state.latitude) {
        memcpy(&this->state.latitude, &data.latitude, sizeof(float));
        updates.append("latitude ");
    }

    if (data.longitude != this->state.longitude) {
        memcpy(&this->state.longitude, &data.longitude, sizeof(float));
        updates.append("longitude ");
    }

    if (data.true_north != this->state.true_north) {
        memcpy(&this->state.true_north, &data.true_north, sizeof(float));
        updates.append("true_north ");
    }

    if (data.ground_speed != this->state.ground_speed) {
        memcpy(&this->state.ground_speed, &data.ground_speed, sizeof(float));
        updates.append("ground_speed ");
    }

    if (data.altitude != this->state.altitude) {
        memcpy(&this->state.altitude, &data.altitude, sizeof(float));
        updates.append("altitude ");
    }

    if (updates.length() > 0) {
        this->snf((char*)updates.c_str(), (char*)"state", this);
    }
};

// END

// Database Initialization Registry

void Database::LoadSettings() {
    Settings s;
    memset(&s, 0, sizeof(s));
    
    s.led_status = storage->ReadU16("led_status");
    s.serial_tx_active = storage->ReadU8("se_tx_active");

    this->UpdateSettings(s);
    this->SettingsLoaded = true;
}

void Database::LoadState() {
    State s;
    memset(&s, 0, sizeof(s));

    s.gnss_fix_quality = 0;
    s.gnss_fix_type = 0;
    s.sat_count_gps = 0;
    s.sat_count_glonass = 0;
    s.longitude = 0.00;
    s.latitude = 0.00;
    s.true_north = 0.00;
    s.ground_speed = 0.00;
    s.altitude = 0.00;

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