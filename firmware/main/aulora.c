#include "aulora.h"

void hash_str(const uint8_t* hash, char* str_hash) {
  for (int i = 0; i < HASH_LENGTH; i++) {
    sprintf(str_hash + (i * 2), "%02x", hash[i]);
  }
}

int is_null(void* ptr) {
  if (ptr == NULL) return 1;
  return 0;
}

uint8_t* file_hash(uint8_t* file_name) {
  FILE* fp = fopen((void*)file_name, "rb");

  if (is_null(fp)) return NULL;

  int bytes;
  static uint8_t c[HASH_LENGTH] = {0};
  uint8_t buf[256];

  mbedtls_md5_context ctx;
  mbedtls_md5_init(&ctx);
  mbedtls_md5_starts(&ctx);

  while ((bytes = fread(buf, 1, 256, fp)) != 0) {
    mbedtls_md5_update(&ctx, buf, bytes);
  }

  mbedtls_md5_finish(&ctx, c);
  mbedtls_md5_free(&ctx);

  fclose(fp);
  return (uint8_t*)&c;
}

size_t file_size(uint8_t* file_name) {
  size_t size;
  FILE* fp = fopen((void*)file_name, "rb");

  if (is_null(fp)) return -1;

  fseek(fp, 0L, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  fclose(fp);

  return size;
}

size_t load_file(uint8_t* buffer, uint8_t* file_name) {
  FILE* fp = fopen((void*)file_name, "rb");

  if (is_null(fp)) return -1;

  fseek(fp, 0L, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  fread(buffer, size, 1, fp);
  fclose(fp);

  return size;
}

size_t file_chunk(uint8_t* buffer, uint8_t* file_name, size_t size,
                 size_t chunk) {
  FILE* fp = fopen((void*)file_name, "rb");

  if (is_null(fp)) return -1;

  size_t chunk_len = MAX_PAYLOAD_SIZE;
  size_t chunk_past = chunk_len * chunk;

  if (chunk_len + chunk_past > size) chunk_len = size - chunk_past;

  fseek(fp, chunk_past, SEEK_SET);
  fread(buffer, chunk_len, 1, fp);
  fclose(fp);

  return chunk_len;
}

int create_pod(sqlite3* db, Pod pod) {
  uint32_t id = (uint32_t)rand();
  char pod_create_sql[512];

  snprintf(pod_create_sql, sizeof(pod_create_sql),
           "INSERT INTO %s VALUES (%d, '%s', %f, %f, %d, %f, %f, %d, %d, %d, "
           "%d, %d);",
           PODS, id, pod.sender, pod.source->latitude, pod.source->longitude,
           pod.source->altitude, pod.destination->latitude,
           pod.destination->longitude, pod.destination->altitude, pod.timestamp,
           pod.flight_radius, pod.priority, pod.hops);

  if (SQLITE_OK != sqlite3_exec(db, pod_create_sql, NULL, NULL, NULL)) {
    printf("Error: Creating Pod SQL. Err: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  return (int)id;
}

int create_payload(sqlite3* db, int pod_id, Payload pl) {
  uint32_t id = (uint32_t)rand();
  char pl_create_sql[512];

  snprintf(pl_create_sql, sizeof(pl_create_sql),
           "INSERT INTO %s VALUES (%d, %d, %d, %d, ?, ?);", PAYLOADS, id,
           (uint32_t)pod_id, pl.chunk, pl.chunk_num);

  int rc;
  sqlite3_stmt* stmt = NULL;
  rc = sqlite3_prepare_v2(db, pl_create_sql, -1, &stmt, 0);
  rc = sqlite3_bind_blob(stmt, 1, pl.hash_key.data, pl.hash_key.len, NULL);
  rc = sqlite3_bind_blob(stmt, 2, pl.data.data, pl.data.len, NULL);

  if (SQLITE_OK != rc) {
    printf("Error: Creating Payloads SQL. Err: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return (int)id;
}

int find_payload(sqlite3* db, uint32_t chunk, uint8_t* hash, size_t len) {
  char pl_create_sql[512];

  snprintf(pl_create_sql, sizeof(pl_create_sql),
           "SELECT * FROM payloads WHERE hash_key == ? AND chunk == %d", chunk);

  int rc;
  sqlite3_stmt* stmt = NULL;
  rc = sqlite3_prepare_v2(db, pl_create_sql, -1, &stmt, 0);
  rc = sqlite3_bind_blob(stmt, 1, hash, len, NULL);

  if (rc != SQLITE_OK) {
    printf("Error: Finding Payloads Hash. Err: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    return 1;
  }

  sqlite3_finalize(stmt);
  return 0;
}

int tx_payload(sqlite3* db, uint8_t* file_name) {
  printf("[TX_PAYLOAD] Transmitting payload...\n");

  size_t size = file_size(file_name);
  uint8_t* hash = file_hash(file_name);

  printf("Payload size is %d.\n", (int)size);
  printf("Payload MD5 hash is: ");
  for (int i = 0; i < HASH_LENGTH; i++) {
    printf("%02x", hash[i]);
  }
  printf("\n");

  size_t chunk_num = (size + MAX_PAYLOAD_SIZE - 1) / MAX_PAYLOAD_SIZE;

  printf("Splitting payload into %d chunks of size %d.\n", (int)chunk_num,
         MAX_PAYLOAD_SIZE);
  printf("+ Generating Pod.\n");

  Pod pod = POD__INIT;
  Location loc = LOCATION__INIT;

  loc.latitude = 108.5038888;
  loc.longitude = 45.7811111;
  loc.altitude = 560;

  pod.sender = SENDER;
  pod.source = &loc;
  pod.destination = &loc;
  pod.timestamp = (uint32_t)time(NULL);
  pod.flight_radius = 50;
  pod.priority = 10;
  pod.hops = 0;

  int id = -1;
  if ((id = create_pod(db, pod)) == -1) {
    return 3;
  }

  for (int i = 0; i < chunk_num; i++) {
    printf("  Generating Payload %d/%d.\n", i + 1, (int)chunk_num);

    uint8_t data_buf[MAX_PAYLOAD_SIZE];
    size_t data_len = file_chunk((uint8_t*)&data_buf, file_name, size, i);

    Payload pl = PAYLOAD__INIT;

    pl.chunk = i;
    pl.chunk_num = chunk_num;
    pl.hash_key.data = hash;
    pl.hash_key.len = HASH_LENGTH;
    pl.data.data = data_buf;
    pl.data.len = data_len;

    if (create_payload(db, id, pl) == -1) {
      return 3;
    }

    pod.payload = &pl;

    size_t pod_len = pod__get_packed_size(&pod);
    uint8_t* pod_buf = (uint8_t*)malloc(pod_len);
    if (is_null(pod_buf)) return 1;

    pod__pack(&pod, pod_buf);

    uint8_t filename[32];
    sprintf((void*)filename, "/spiffs/rx_cache/small-%d.pb", i);
    FILE* fp = fopen((void*)filename, "wb");
    if (is_null(fp)) return 1;

    fwrite(pod_buf, pod_len, 1, fp);
    fclose(fp);
    free(pod_buf);
  }

  return 0;
}

int rx_payload(sqlite3* db, Pod* pod) {
  printf("[RX_PAYLOAD] Parsing payload...\n");

  int r = find_payload(db, pod->payload->chunk, pod->payload->hash_key.data,
                      pod->payload->hash_key.len);

  if (!r) {
    char str[HASH_LENGTH * 2];
    hash_str(pod->payload->hash_key.data, (char*)&str);

    printf("[RX_PAYLOAD] New payload identified.\n");
    printf("Chunk %d (MD5: %s)\n", pod->payload->chunk, str);

    int id = -1;
    if ((id = create_pod(db, *pod)) == -1) {
      return 3;
    }

    if (create_payload(db, id, *(pod->payload)) == -1) {
      return 3;
    }
  } else {
    printf("[RX_PAYLOAD] Repeated payload, ignoring...\n");
  }

  return -1;
}

int rx_request(sqlite3* db, Pod* pod) {
  printf("[RX_REQUEST] Parsing request...\n");
  return -1;
}

int validate_payload(Payload* pl) {
  int err = 0;

  err += (pl->chunk > pl->chunk_num);
  err += (pl->hash_key.len == 0);
  err += (pl->data.len == 0);

  return err;
}

int validate_request(Request* req) {
  int err = 0;

  err += (req->hash_key.len == 0);

  return err;
}

int validate_pod(Pod* pod) {
  int err = 0;

  err += (pod->payload != NULL && pod->request != NULL);
  err += (pod->payload == NULL && pod->request == NULL);
  err += (pod->source == NULL || pod->destination == NULL);
  err += (pod->priority < 0 && pod->priority > 10);
  err += strlen(pod->sender) == 0;

  return err;
}

int receive(sqlite3* db, char* file_name) {
  uint8_t payload[MAX_POD_SIZE];
  size_t size = load_file((uint8_t*)&payload, (void*)file_name);

  if (size == -1) {
    printf("Error: Opening the payload file.\n");
    return 1;
  }

  Pod* pod = pod__unpack(NULL, size, (uint8_t*)&payload);
  if (is_null(pod)) return 2;

  int err = 0;

  if ((err = validate_pod(pod))) {
    printf("[RX] Invalid packet (%d).\n", err);
    return 1;
  };

  if (pod->payload != NULL) {
    if ((err = validate_payload(pod->payload))) {
      printf("[RX] Invalid payload (%d).\n", err);
      return 1;
    };

    rx_payload(db, pod);
  }

  if (pod->request != NULL) {
    if ((err = validate_request(pod->request))) {
      printf("[RX] Invalid request (%d).\n", err);
      return 1;
    };

    rx_request(db, pod);
  }

  pod__free_unpacked(pod, NULL);

  if (remove(file_name)) {
    printf("Error: Removing cache file.\n");
  }

  return 0;
}

int exec(char* query, sqlite3* db, void (*worker)(sqlite3_stmt*, sqlite3*)) {
  sqlite3_stmt* stmt = NULL;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, 0) != SQLITE_OK) {
    printf("Error: Finding Payloads Hash. Err: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    worker(stmt, db);
  }

  sqlite3_finalize(stmt);
  return 0;
}

void keys(sqlite3_stmt* stmt, sqlite3* db) {
  int chunk = sqlite3_column_int(stmt, 1);
  const uint8_t* hash = sqlite3_column_blob(stmt, 0);

  char str[HASH_LENGTH * 2];
  hash_str(hash, (char*)&str);

  char sql[] =
      "SELECT data, (SELECT count(*) FROM payloads WHERE hash_key == ?) FROM "
      "payloads WHERE hash_key == ? AND processed == 0 ORDER BY chunk ASC";

  int rc;
  sqlite3_stmt* stmt2 = NULL;

  rc = sqlite3_prepare_v2(db, sql, -1, &stmt2, 0);
  rc = sqlite3_bind_blob(stmt2, 1, hash, HASH_LENGTH, NULL);
  rc = sqlite3_bind_blob(stmt2, 2, hash, HASH_LENGTH, NULL);

  if (rc != SQLITE_OK) {
    printf("Error: Finding Payloads. Err: %s\n", sqlite3_errmsg(db));
    return;
  }

  char filename[64];
  sprintf(filename, "/spiffs/%.*s", 24, str);

  while (sqlite3_step(stmt2) == SQLITE_ROW) {
    int c = sqlite3_column_int(stmt2, 1);

    if (c != chunk) {
      sqlite3_finalize(stmt2);
      return;
    }

    size_t size = sqlite3_column_bytes(stmt2, 0);
    const uint8_t* payload = sqlite3_column_blob(stmt2, 0);

    FILE* fp = fopen(filename, "ab");
    if (is_null(fp)) return;

    fseek(fp, chunk * MAX_PAYLOAD_SIZE, SEEK_SET);
    fwrite(payload, size, 1, fp);
    fclose(fp);
  }

  printf("File Generated: %s\n", filename);
  sqlite3_finalize(stmt2);
};

int restore_payloads(sqlite3* db) {
  return exec(
      "SELECT DISTINCT hash_key, chunk_num FROM payloads ORDER BY chunk ASC",
      db, keys);
}