#include "database.h"

char* ConvertToString(uint16_t dat) {
    static char *p,buf[10];
    sprintf(buf, "%d", (int)dat);
    p = buf;
    return p;
}

void Database::RegisterSocketNotifier(SocketNotifier snf) {
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
        this->snf("ws_update_rate", "settings", ConvertToString(this->settings.ws_update_rate));
    }
};

void Database::UpdateState(State data) {

};

void Database::LoadSettings() {
    this->settings.ws_update_rate = storage->ReadU32("ws_update_rate");
}

void Database::LoadState() {
    
}

Database::Database(KeyStorage* storage) {
    this->storage = storage;

    LoadSettings();
    LoadState();
}