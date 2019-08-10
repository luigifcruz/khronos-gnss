#ifndef AULORA_H
#define AULORA_H

#include <dirent.h>
#include <inttypes.h>
#include <mbedtls/md5.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aulora.pb-c.h"

#define HASH_LENGTH 16
#define MAX_PAYLOAD_SIZE 512
#define MAX_HEADER_SIZE 128
#define MAX_POD_SIZE MAX_PAYLOAD_SIZE + MAX_HEADER_SIZE
#define SENDER "PU2SPY"

#define PODS                                                                 \
  "pods (id, sender, src_lat, src_lon, src_alt, dst_lat, dst_lon, dst_alt, " \
  "timestamp, flight_radius, priority, hops)"
#define PAYLOADS "payloads(id, payload_pod, chunk, chunk_num, hash_key, data)"

int is_null(void*);
int tx_payload(sqlite3*, uint8_t*);
int receive(sqlite3*, char*);
int restore_payloads(sqlite3*);

#endif