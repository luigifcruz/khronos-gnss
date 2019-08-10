#include <dirent.h>
#include <inttypes.h>
#include <mbedtls/md5.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../firmware/main/aulora.h"

int main() {
  srand(time(NULL));

  sqlite3* db_tx;
  sqlite3_open("./spiffs/database_tx.db", &db_tx);
  if (is_null(db_tx)) return 2;

  sqlite3* db_rx;
  sqlite3_open("./spiffs/database_rx.db", &db_rx);
  if (is_null(db_rx)) return 2;

  uint8_t* tx_fn = "/home/luigifcruz/Downloads/gogog.go.gz";
  if (tx_payload(db_tx, tx_fn)) {
    printf("Error: Transmitting the payload.\n");
  }

  char* rx_cache = "./spiffs/rx_cache";
  DIR* dir = opendir(rx_cache);
  if (is_null(dir)) return 2;

  struct dirent* ent;
  while ((ent = readdir(dir)) != NULL) {
    if (strstr(ent->d_name, ".pb")) {
      char rx_fn[512];  // Temporary fix for the sprintf warning.
      sprintf(rx_fn, "%s/%s", rx_cache, ent->d_name);

      if (receive(db_rx, rx_fn)) {
        printf("Error: Parsing the received payload.\n");
      }
    }
  }
  closedir(dir);

  restore_payloads(db_rx);

  sqlite3_close(db_rx);
  sqlite3_close(db_tx);
  return 0;
}
