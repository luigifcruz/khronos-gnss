#ifndef NTPSERVER_H
#define NTPSERVER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "soc/rtc_periph.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
//#include "esp32/ulp.h"
//#include "ulp_main.h"

#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/udp.h"

#include <netdb.h>
#include <time.h>
#include <sys/time.h>

#include "ntp_handler.h"

#define BUFFLEN 512
#define NTPPORT 123

class NtpServer {
private:
    static void InitCoprocessor();
    static void UdpHandler(void* pvParameters);

public:
	static void GetTime(tstamp* time);

    NtpServer();
};

#endif
