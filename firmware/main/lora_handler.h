#ifndef LORA_HANDLER_H
#define LORA_HANDLER_H
#define __STDC_FORMAT_MACROS

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "database.h"

#include "aulora.pb-c.h"

class LoraHandler
{
private:
    Database *db;

    static void LoraChannel(void *pvParameters);

public:
    LoraHandler(Database *db);
};

#endif