#ifndef LORA_HANDLER_H
#define LORA_HANDLER_H
#define __STDC_FORMAT_MACROS

#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "database.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"

extern "C" {
#include "aulora.h"
}

class LoraHandler {
 private:
  Database *db;

  static void LoraChannel(void *pvParameters);

 public:
  LoraHandler(Database *db);
};

#endif