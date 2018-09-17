#ifndef DATABASE_H
#define DATABASE_H

#include <cstring>

#include "esp_event_loop.h"
#include "esp_system.h"
#include "key_storage.h"

typedef struct {
    uint8_t gps_fix_quality;
    uint8_t gps_fix_type;
    uint8_t gps_sat_numb;
    uint8_t glonass_sat_numb;
} State;

typedef struct {
    uint16_t ws_update_rate;
    uint16_t led_status;
    uint8_t serial_tx_active;
} Settings;

typedef void (*Notifier)(char* key, char* zone, void* value);

class Database {
    private:
    Settings settings;
    State state;
    KeyStorage* storage;
    bool SettingsLoaded = false;
    bool StateLoaded = false;
    
    Notifier nf;
    Notifier snf;

    public:
    Database(KeyStorage* storage);

    Settings GetSettings();
    State GetState();

    void UpdateSettings(Settings data);
    void UpdateState(State data);

    void LoadSettings();
    void LoadState();

    void RegisterNotifier(char* key, Notifier nf);
};

#endif