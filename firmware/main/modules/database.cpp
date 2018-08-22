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
};

void Database::UpdateState(State data) {

};

int Database::Init(KeyStorage* storage) {
    this->storage = storage;
    return 0;
}