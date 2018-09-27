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
    static void ParseNMEA(std::string lines, void* pvParameters, time_t* then);
    static void ProgramUBX();
    static void sendUBXCommand(char *command, int size);

    public:
    GpsHandler(Database* db);
};

#endif