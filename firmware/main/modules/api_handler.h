#ifndef API_HANDLER_H
#define API_HANDLER_H

#include <string>
#include <iostream>

#include "freertos/FreeRTOS.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "esp_log.h"
#include "cJSON.h"

#include "database.h"

class ApiHandler {
    private:
    static void AddKey(char* key, char* zone, char* value, cJSON* dest);

    public:
    static char* Request(cJSON* req, Database* db, bool* broadcast);
    static char* Response(char* key, char* zone, Database* db);
};

#endif