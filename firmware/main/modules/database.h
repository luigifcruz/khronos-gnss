#ifndef DATABASE_H
#define DATABASE_H

#include <cstring>
#include <string>
#include <iostream>

#include "esp_event_loop.h"
#include "esp_system.h"
#include "key_storage.h"

typedef struct {
    uint8_t gnss_fix_quality;
    uint8_t gnss_fix_type;
    uint8_t sat_count_gps;
    uint8_t sat_count_glonass;
    uint8_t sat_count_galileo;
    float longitude;
    float latitude;
    float true_north;
    float ground_speed;
    float altitude;
} State;

typedef struct {
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