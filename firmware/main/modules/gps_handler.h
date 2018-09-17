#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "database.h"

extern "C" {
#include "minmea/minmea.h"
}

#define ECHO_TEST_TXD  (GPIO_NUM_17)
#define ECHO_TEST_RXD  (GPIO_NUM_16)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define NMEA_BUF_SIZE 1024
#define INDENT_SPACES "  "

class GpsHandler {
    private:
    Database* db;

    static void GpsChannel(void* pvParameters);
    static void PrintNMEA(char* line);
    static void ParseNMEA(char* line, void* pvParameters);
    static void ProgramUBX();

    public:
    GpsHandler(Database* db);
};

#endif