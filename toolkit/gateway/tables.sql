CREATE TABLE IF NOT EXISTS "pods" (
    "id" integer DEFAULT '',
    "sender" text NOT NULL,
    "src_lat" real NOT NULL,
    "src_lon" real NOT NULL,
    "src_alt" real NOT NULL,
    "dst_lat" real NOT NULL,
    "dst_lon" real NOT NULL,
    "dst_alt" real NOT NULL,
    "timestamp" integer NOT NULL,
    "flight_radius" integer NOT NULL,
    "priority" integer NOT NULL,
    "hops" integer NOT NULL,
    "hops" integer NOT NULL,
    "valid" integer DEFAULT 1,
    "complete" integer DEFAULT 0,
    PRIMARY KEY (id));

CREATE TABLE IF NOT EXISTS "payloads" (
    "id" integer DEFAULT '',
    "payload_pod" integer NOT NULL,
    "chunk" integer NOT NULL,
    "chunk_num" integer NOT NULL,
    "hash_key" blob NOT NULL,
    "data" blob NOT NULL, 
    PRIMARY KEY (id)
    FOREIGN KEY(payload_pod) REFERENCES pods(id));

INSERT INTO pods("sender", "src_lat", "src_lon", "src_alt", "dst_lat", "dst_lon", "dst_alt", "timestamp", "flight_radius", "priority", "hops") 
VALUES ("PU2SPY", 00000000, 00000000, 000, 00000000, 00000000, 000, 1563241234, 10, 1, 0);

INSERT INTO payloads("payload_pod", "chunk_num", "hash_key") 
VALUES (1, 3, "X6AJGL6HwVYXGC0OTQKtQ0kbip0OkErN");

INSERT INTO chunks("chunk_payload", "data") 
VALUES (1, "X6AJGL6HwVYXGC0OTQKtQ0kbip0OkErNBC");

SELECT * FROM pods
JOIN payloads ON pods.pod_id = payloads.payload_pod
JOIN chunks ON payloads.payload_id = chunks.chunk_payload;
