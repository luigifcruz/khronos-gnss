#include "database.h"

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
    }

    if (data.led_status != this->settings.led_status) {
        storage->WriteU32("led_status", data.led_status);
        memcpy(&this->settings.led_status, &data.led_status, sizeof(Settings));
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