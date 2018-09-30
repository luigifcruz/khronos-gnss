#ifndef NTPHANDLER_H
#define NTPHANDLER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"

#include <string.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>

typedef struct {
    uint32_t seconds;
    uint32_t fraction;
} tstamp;

typedef struct {
	char		flags;		    /* ntp flags*/
    char    	stratum;        /* stratum */
    char    	poll;           /* poll interval */
    uint8_t	  	precision;      /* precision */
    uint32_t	rootdelay;      /* root delay */
    uint32_t   	rootdisp;       /* root dispersion */
    char    	refid[4];       /* reference ID */
    tstamp  	reftime;        /* reference time */
    tstamp  	org;            /* origin timestamp */
    tstamp  	rec;            /* receive timestamp */
    tstamp  	xmt;            /* transmit timestamp */
} NtpPacket;

typedef void (*GetTime)(tstamp* time);

class NtpHandler {
private:
    NtpPacket* ntp;

    bool IsFilled = false; 

    bool IsValid();

public:
    int FromBinary(char* dat, unsigned int size);
    char* Reply(tstamp recv_time, GetTime getTime);
    void ToUNIX(tstamp ntp_t, time_t* unix_t);
    void Print();

    NtpHandler();
};

#endif
