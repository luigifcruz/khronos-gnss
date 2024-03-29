#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <time.h>
#include <sys/time.h>

#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "database.h"
#include "cJSON.h"

extern "C" {
#include "minmea/minmea.h"
}

#define NMEA_RATE 10
#define NMEA_BUF_SIZE 1024
#define INDENT_SPACES "  "

class GpsHandler {
    private:
    Database* db;

    static void GpsChannel(void* pvParameters);
    static void ParseNMEA(std::string lines, void* pvParameters);
    static void ProgramUBX();
    static void sendUBXCommand(char *command, int size);

    public:
    GpsHandler(Database* db);
};

#endif