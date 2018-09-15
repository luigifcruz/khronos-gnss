#include "ntp_handler.h"

bool NtpHandler::IsValid() {
	return this->IsFilled;
}

void NtpHandler::ToUNIX(tstamp ntp_t, time_t* unix_t) {
	*unix_t = ntp_t.seconds - 2208988800UL;

	/*
	time_t now;
    struct tm timeinfo;
    char strftime_buf[64];
    ToUNIX(ntp->xmt, &now);
	localtime_r(&now, &timeinfo);
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
	ESP_LOGI(CONFIG_SN, "The current date/time in Brasilia is: %s", strftime_buf);
	*/
}

void NtpHandler::Print() {
	if (!this->IsValid()) {
		return;
	}

	printf("Peer Clock Stratum: %d\n", ntp->stratum);
	printf("Peer Polling Interval: %d\n", ntp->poll);
	printf("Peer Clock Precision: %d\n", ntp->precision);
    printf("Root Delay: %u second(s)\n", ntp->rootdelay - 65535);
    printf("Root Dispersion: %u second(s)\n", ntp->rootdisp - 65535);
    printf("Reference ID: %s\n", ntp->refid);
    printf("Reference Timestap: %08x seconds %08x fraction\n", ntp->reftime.seconds, ntp->reftime.fraction);
    printf("Origin Timestap: %08x seconds %08x fraction\n", ntp->org.seconds, ntp->org.fraction);
    printf("Receive Timestap: %08x seconds %08x fraction\n", ntp->rec.seconds, ntp->rec.fraction);
    printf("Transmit Timestap: %08x seconds %08x fraction\n", ntp->xmt.seconds, ntp->xmt.fraction);
}

int NtpHandler::FromBinary(char* dat, unsigned int size) {
	if (sizeof(ntp) > size) {
		return 1;
	}

    memcpy(this->ntp, dat, sizeof(NtpPacket));

    ntp->rootdelay = ntohl(ntp->rootdelay);
    ntp->rootdisp = ntohl(ntp->rootdisp);

    ntp->reftime.seconds = ntohl(ntp->reftime.seconds);
    ntp->reftime.fraction = ntohl(ntp->reftime.fraction);

    ntp->org.seconds = ntohl(ntp->org.seconds);
    ntp->org.fraction = ntohl(ntp->org.fraction);

    ntp->rec.seconds = ntohl(ntp->rec.seconds);
    ntp->rec.fraction = ntohl(ntp->rec.fraction);

    ntp->xmt.seconds = ntohl(ntp->xmt.seconds);
    ntp->xmt.fraction = ntohl(ntp->xmt.fraction);

    this->IsFilled = true;
    return 0;
}

char* NtpHandler::Reply(tstamp recv_time, GetTime getTime) {
	ntp->flags = 0b11000000 | 0b00100000 | 0b00000100;
	ntp->stratum = 0x01;
	ntp->precision = 0x00;
	strncpy((char*)ntp->refid, "GPS", sizeof(ntp->refid));
	ntp->org  = ntp->xmt;
	ntp->rec = recv_time;
	getTime(&ntp->reftime);

	ntp->rootdelay = htonl(ntp->rootdelay);
    ntp->rootdisp = htonl(ntp->rootdisp);

    ntp->reftime.seconds = htonl(ntp->reftime.seconds);
    ntp->reftime.fraction = htonl(ntp->reftime.fraction);

    ntp->org.seconds = htonl(ntp->org.seconds);
    ntp->org.fraction = htonl(ntp->org.fraction);

    ntp->rec.seconds = htonl(ntp->rec.seconds);
    ntp->rec.fraction = htonl(ntp->rec.fraction);

    getTime(&ntp->xmt);

    ntp->xmt.seconds = htonl(ntp->xmt.seconds);
    ntp->xmt.fraction = htonl(ntp->xmt.fraction);

	return (char*)ntp;
}

NtpHandler::NtpHandler() {
	static NtpPacket nt;
	this->ntp = &nt;
}