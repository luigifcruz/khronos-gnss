#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "esp_system.h"
#include "esp_log.h"

extern "C" {
#include "minmea/minmea.h"
}


class GpsHandler {
    private:

    public:
    GpsHandler();
};

#endif