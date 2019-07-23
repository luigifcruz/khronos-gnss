#include <stdio.h>
#include <sqlite3.h>
#include <time.h>
#include <mbedtls/md5.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "aulora.pb-c.h"

#define HASH_LENGTH 16
#define MAX_PAYLOAD_SIZE 256
#define MAX_HEADER_SIZE 128
#define MAX_POD_SIZE MAX_PAYLOAD_SIZE + MAX_HEADER_SIZE
#define SENDER "PU2SPY"

#define PODS "pods (id, sender, src_lat, src_lon, src_alt, dst_lat, dst_lon, dst_alt, timestamp, flight_radius, priority, hops)"
#define PAYLOADS "payloads(id, payload_pod, chunk, chunk_num, hash_key, data)"

int nullPointer(void* ptr) {
    if (ptr == NULL)
        return 1;
    return 0;
}

uint8_t* fileHash(char* file_name) {
    FILE *fp = fopen(file_name, "rb");
    
    if (nullPointer(fp))
	return NULL;

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

size_t fileSize(char* file_name) {
    size_t size;
    FILE *fp = fopen(file_name, "rb");
    
    if (nullPointer(fp))
	return -1;

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    fclose(fp);

    return size;
}

size_t loadFile(uint8_t* buffer, char* file_name) {
    FILE *fp = fopen(file_name, "rb");
    
    if (nullPointer(fp))
	return -1;

    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
  
    fread(buffer, size, 1, fp);
    fclose(fp);

    return size;
}

size_t fileChunk(uint8_t* buffer, char* file_name, size_t size, size_t chunk) {
    FILE *fp = fopen(file_name, "rb");
    
    if (nullPointer(fp))
	return -1;
 
    size_t chunk_len = MAX_PAYLOAD_SIZE;
    size_t chunk_past = chunk_len*chunk;
    
    if (chunk_len+chunk_past > size)
        chunk_len = size-chunk_past;
 
    fseek(fp, chunk_past, SEEK_SET); 
    fread(buffer, chunk_len, 1, fp);
    fclose(fp);

    return chunk_len;
}

int createPod(sqlite3* db, Pod pod) {
    uint32_t id = (uint32_t)rand();
    char pod_create_sql[512];

    snprintf(pod_create_sql, sizeof(pod_create_sql),
             "INSERT INTO %s VALUES (%d, '%s', %f, %f, %d, %f, %f, %d, %d, %d, %d, %d);",
             PODS, id, pod.sender,
             pod.source->latitude, pod.source->longitude, pod.source->altitude,
             pod.destination->latitude, pod.destination->longitude, pod.destination->altitude,
             pod.timestamp, pod.flight_radius, pod.priority, pod.hops);

    if (SQLITE_OK != sqlite3_exec(db, pod_create_sql, NULL, NULL, NULL)) {
        printf("Error: Creating Pod SQL. Err: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    return (int)id;
}

int createPayload(sqlite3* db, int pod_id, Payload pl) {
    uint32_t id = (uint32_t)rand();
    char pl_create_sql[512];

    snprintf(pl_create_sql, sizeof(pl_create_sql),
             "INSERT INTO %s VALUES (%d, %d, %d, %d, ?, ?);",
             PAYLOADS, id, (uint32_t)pod_id, pl.chunk, pl.chunk_num);

    int rc;
    sqlite3_stmt *stmt = NULL;
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

int txPayload(uint8_t* file_name) {
    printf("[TX_PAYLOAD] Transmitting payload...\n");

    sqlite3 *db;
    sqlite3_open("./spiffs/database.db", &db);

    if (nullPointer(db))
	return 2;

    size_t size = fileSize(file_name);
    uint8_t* hash = fileHash(file_name);
    
    printf("Payload size is %ld.\n", size);
    printf("Payload MD5 hash is: ");
    for(int i = 0; i < HASH_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n"); 

    size_t chunk_num = (size_t)(((float)size/(float)MAX_PAYLOAD_SIZE)+0.5);

    printf("Splitting payload into %ld chunks of size %d.\n", chunk_num, MAX_PAYLOAD_SIZE);
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
    if ((id = createPod(db, pod)) == -1) {
        return 3;
    }

    for (int i=0; i<chunk_num; i++) {
        printf("  Generating Payload %d/%ld.\n", i+1, chunk_num);
        
        uint8_t data_buf[MAX_PAYLOAD_SIZE];
        size_t data_len = fileChunk((uint8_t*)&data_buf, file_name, size, i);
        
        Payload pl = PAYLOAD__INIT;
        
        pl.chunk = i;
        pl.chunk_num = chunk_num;
        pl.hash_key.data = hash;
        pl.hash_key.len = HASH_LENGTH;
        pl.data.data = data_buf;
        pl.data.len = data_len;

        if (createPayload(db, id, pl) == -1) {
            return 3;
        }
        
        pod.payload = &pl;

        size_t pod_len = pod__get_packed_size(&pod);
        uint8_t* pod_buf = (uint8_t*)malloc(pod_len);
   
        if (nullPointer(pod_buf))
            return 1;
   
        pod__pack(&pod, pod_buf);
        
        uint8_t filename[32];
        sprintf(filename, "./spiffs/rx_cache/small-%d.pb", i);
        FILE *fp = fopen(filename, "wb");
        
        if (nullPointer(fp))
            return 1;
        
        fwrite(pod_buf, pod_len, 1, fp);
        fclose(fp);    
        free(pod_buf);
    }

    return 0;
}

int rxPayload(Pod* pod) {
    printf("[RX_PAYLOAD] Parsing payload...\n");
    return -1;
}

int rxRequest(Pod* pod) {
    printf("[RX_REQUEST] Parsing request...\n");
    return -1;
}

int validatePayload(Payload* pl) {
    int err = 0;

    err += (pl->chunk > pl->chunk_num);
    err += (pl->hash_key.len == 0);
    err += (pl->data.len == 0);

    return err;
}

int validateRequest(Request* req) {
    int err = 0;

    err += (req->hash_key.len == 0);

    return err;
}

int validatePod(Pod* pod) {
    int err = 0;

    err += (pod->payload != NULL && pod->request != NULL);
    err += (pod->payload == NULL && pod->request == NULL);
    err += (pod->source == NULL || pod->destination == NULL);
    err += (pod->priority < 0 && pod->priority > 10);
    err += strlen(pod->sender) == 0;
    
    return err;
}

int receive(char* file_name) {
    printf("[RX] New message received...\n");

    uint8_t payload[MAX_POD_SIZE];
    size_t size = loadFile((uint8_t*)&payload, file_name);

    if (size == -1) {
        printf("Error: Opening the payload file.\n");	
        return 1;
    }

    Pod* pod = pod__unpack(NULL, size, (uint8_t*)&payload);
    if (nullPointer(pod))
	return 2;

    int err = 0;

    if ((err = validatePod(pod))) {
        printf("[RX] Invalid packet (%d).\n", err);
        return 1;
    };

    if (pod->payload != NULL) {
        if ((err = validatePayload(pod->payload))) {
            printf("[RX] Invalid payload (%d).\n", err);
            return 1;
        };

        rxPayload(pod);
    }

    if (pod->request != NULL) {
        if ((err = validateRequest(pod->request))) {
            printf("[RX] Invalid request (%d).\n", err);
            return 1;
        };

        rxRequest(pod);
    }
    
    pod__free_unpacked(pod, NULL);

    if (remove(file_name)) {
        printf("Error: Removing cache file.\n");
    }

    return 0;
}

int main() {
    srand(time(NULL));

    uint8_t *tx_fn = "./spiffs/small.txt.gz";
    if (txPayload(tx_fn)) {
        printf("Error: Transmitting the payload.\n");
    }

    char* rx_cache = "./spiffs/rx_cache";        
    DIR *dir = opendir(rx_cache);
    if (nullPointer(dir))
	return 2;
    
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, ".pb")) {
            char rx_fn[512]; // Temporary fix for the sprintf warning. 
            sprintf(rx_fn, "%s/%s", rx_cache, ent->d_name);
            
            if (receive(rx_fn)) {
                printf("Error: Parsing the received payload.\n");
            }
        }        
    }
    closedir(dir);

    return 0;
}
