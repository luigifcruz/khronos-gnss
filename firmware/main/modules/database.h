#ifndef DATABASE_H
#define DATABASE_H

#include <cstring>

#include "esp_event_loop.h"
#include "esp_system.h"

#include "key_storage.h"

typedef struct {
    uint32_t fs_frequency;
} State;

typedef struct {
    uint16_t ws_update_rate;
} Settings;

class Database {
    private:
    Settings settings;
    State state;
    KeyStorage* storage;

    public:
    int Init(KeyStorage* storage);

    Settings GetSettings();
    State GetState();

    void UpdateSettings(Settings data);
    void UpdateState(State data);
};

#endif