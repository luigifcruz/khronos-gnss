/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: aulora.proto */

#ifndef PROTOBUF_C_aulora_2eproto__INCLUDED
#define PROTOBUF_C_aulora_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003002 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Main__Location Main__Location;
typedef struct _Main__Payload Main__Payload;
typedef struct _Main__Request Main__Request;
typedef struct _Main__Pod Main__Pod;
typedef struct _Main__Data Main__Data;
typedef struct _Main__Plane Main__Plane;
typedef struct _Main__Vessel Main__Vessel;
typedef struct _Main__Vehicle Main__Vehicle;
typedef struct _Main__Balloon Main__Balloon;
typedef struct _Main__Report Main__Report;
typedef struct _Main__Spacecraft Main__Spacecraft;
typedef struct _Main__Weather Main__Weather;
typedef struct _Main__Message Main__Message;
typedef struct _Main__Audio Main__Audio;
typedef struct _Main__File Main__File;


/* --- enums --- */


/* --- messages --- */

struct  _Main__Location
{
  ProtobufCMessage base;
  uint32_t latitude;
  uint32_t longitude;
  uint32_t altitude;
};
#define MAIN__LOCATION__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__location__descriptor) \
    , 0, 0, 0 }


struct  _Main__Payload
{
  ProtobufCMessage base;
  uint32_t chunk;
  uint32_t chunks;
  ProtobufCBinaryData hash;
  ProtobufCBinaryData data;
};
#define MAIN__PAYLOAD__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__payload__descriptor) \
    , 0, 0, {0,NULL}, {0,NULL} }


struct  _Main__Request
{
  ProtobufCMessage base;
  uint32_t chunk;
  ProtobufCBinaryData hash;
};
#define MAIN__REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__request__descriptor) \
    , 0, {0,NULL} }


struct  _Main__Pod
{
  ProtobufCMessage base;
  char *sender;
  uint64_t timestamp;
  Main__Location *source;
  Main__Location *destination;
  uint32_t flight_radius;
  uint32_t priority;
  uint32_t hops;
  Main__Payload *payload;
  Main__Request *request;
};
#define MAIN__POD__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__pod__descriptor) \
    , (char *)protobuf_c_empty_string, 0, NULL, NULL, 0, 0, 0, NULL, NULL }


struct  _Main__Data
{
  ProtobufCMessage base;
  size_t n_report;
  Main__Report **report;
  size_t n_weather;
  Main__Weather **weather;
  size_t n_message;
  Main__Message **message;
  size_t n_audio;
  Main__Audio **audio;
  size_t n_file;
  Main__File **file;
  Main__Balloon *balloon;
  Main__Plane *plane;
  Main__Vessel *vessel;
  Main__Vehicle *vehicle;
  Main__Spacecraft *spacecraft;
};
#define MAIN__DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__data__descriptor) \
    , 0,NULL, 0,NULL, 0,NULL, 0,NULL, 0,NULL, NULL, NULL, NULL, NULL, NULL }


struct  _Main__Plane
{
  ProtobufCMessage base;
  uint32_t wind_speed;
  uint32_t ground_speed;
  char *squawk;
};
#define MAIN__PLANE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__plane__descriptor) \
    , 0, 0, (char *)protobuf_c_empty_string }


struct  _Main__Vessel
{
  ProtobufCMessage base;
  char *name;
};
#define MAIN__VESSEL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__vessel__descriptor) \
    , (char *)protobuf_c_empty_string }


struct  _Main__Vehicle
{
  ProtobufCMessage base;
  uint32_t speed;
};
#define MAIN__VEHICLE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__vehicle__descriptor) \
    , 0 }


struct  _Main__Balloon
{
  ProtobufCMessage base;
  uint32_t speed;
};
#define MAIN__BALLOON__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__balloon__descriptor) \
    , 0 }


struct  _Main__Report
{
  ProtobufCMessage base;
  char *subject;
  char *body;
};
#define MAIN__REPORT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__report__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string }


struct  _Main__Spacecraft
{
  ProtobufCMessage base;
  uint32_t name;
};
#define MAIN__SPACECRAFT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__spacecraft__descriptor) \
    , 0 }


struct  _Main__Weather
{
  ProtobufCMessage base;
  uint64_t timestamp;
  float temperature;
  float humidity;
  float pressure;
};
#define MAIN__WEATHER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__weather__descriptor) \
    , 0, 0, 0, 0 }


struct  _Main__Message
{
  ProtobufCMessage base;
  char *receiver;
  char *body;
  Main__Location *location;
};
#define MAIN__MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__message__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, NULL }


struct  _Main__Audio
{
  ProtobufCMessage base;
  char *receiver;
  char *duration;
  ProtobufCBinaryData blob;
};
#define MAIN__AUDIO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__audio__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, {0,NULL} }


struct  _Main__File
{
  ProtobufCMessage base;
  char *receiver;
  char *name;
  ProtobufCBinaryData blob;
};
#define MAIN__FILE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&main__file__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, {0,NULL} }


/* Main__Location methods */
void   main__location__init
                     (Main__Location         *message);
size_t main__location__get_packed_size
                     (const Main__Location   *message);
size_t main__location__pack
                     (const Main__Location   *message,
                      uint8_t             *out);
size_t main__location__pack_to_buffer
                     (const Main__Location   *message,
                      ProtobufCBuffer     *buffer);
Main__Location *
       main__location__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__location__free_unpacked
                     (Main__Location *message,
                      ProtobufCAllocator *allocator);
/* Main__Payload methods */
void   main__payload__init
                     (Main__Payload         *message);
size_t main__payload__get_packed_size
                     (const Main__Payload   *message);
size_t main__payload__pack
                     (const Main__Payload   *message,
                      uint8_t             *out);
size_t main__payload__pack_to_buffer
                     (const Main__Payload   *message,
                      ProtobufCBuffer     *buffer);
Main__Payload *
       main__payload__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__payload__free_unpacked
                     (Main__Payload *message,
                      ProtobufCAllocator *allocator);
/* Main__Request methods */
void   main__request__init
                     (Main__Request         *message);
size_t main__request__get_packed_size
                     (const Main__Request   *message);
size_t main__request__pack
                     (const Main__Request   *message,
                      uint8_t             *out);
size_t main__request__pack_to_buffer
                     (const Main__Request   *message,
                      ProtobufCBuffer     *buffer);
Main__Request *
       main__request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__request__free_unpacked
                     (Main__Request *message,
                      ProtobufCAllocator *allocator);
/* Main__Pod methods */
void   main__pod__init
                     (Main__Pod         *message);
size_t main__pod__get_packed_size
                     (const Main__Pod   *message);
size_t main__pod__pack
                     (const Main__Pod   *message,
                      uint8_t             *out);
size_t main__pod__pack_to_buffer
                     (const Main__Pod   *message,
                      ProtobufCBuffer     *buffer);
Main__Pod *
       main__pod__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__pod__free_unpacked
                     (Main__Pod *message,
                      ProtobufCAllocator *allocator);
/* Main__Data methods */
void   main__data__init
                     (Main__Data         *message);
size_t main__data__get_packed_size
                     (const Main__Data   *message);
size_t main__data__pack
                     (const Main__Data   *message,
                      uint8_t             *out);
size_t main__data__pack_to_buffer
                     (const Main__Data   *message,
                      ProtobufCBuffer     *buffer);
Main__Data *
       main__data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__data__free_unpacked
                     (Main__Data *message,
                      ProtobufCAllocator *allocator);
/* Main__Plane methods */
void   main__plane__init
                     (Main__Plane         *message);
size_t main__plane__get_packed_size
                     (const Main__Plane   *message);
size_t main__plane__pack
                     (const Main__Plane   *message,
                      uint8_t             *out);
size_t main__plane__pack_to_buffer
                     (const Main__Plane   *message,
                      ProtobufCBuffer     *buffer);
Main__Plane *
       main__plane__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__plane__free_unpacked
                     (Main__Plane *message,
                      ProtobufCAllocator *allocator);
/* Main__Vessel methods */
void   main__vessel__init
                     (Main__Vessel         *message);
size_t main__vessel__get_packed_size
                     (const Main__Vessel   *message);
size_t main__vessel__pack
                     (const Main__Vessel   *message,
                      uint8_t             *out);
size_t main__vessel__pack_to_buffer
                     (const Main__Vessel   *message,
                      ProtobufCBuffer     *buffer);
Main__Vessel *
       main__vessel__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__vessel__free_unpacked
                     (Main__Vessel *message,
                      ProtobufCAllocator *allocator);
/* Main__Vehicle methods */
void   main__vehicle__init
                     (Main__Vehicle         *message);
size_t main__vehicle__get_packed_size
                     (const Main__Vehicle   *message);
size_t main__vehicle__pack
                     (const Main__Vehicle   *message,
                      uint8_t             *out);
size_t main__vehicle__pack_to_buffer
                     (const Main__Vehicle   *message,
                      ProtobufCBuffer     *buffer);
Main__Vehicle *
       main__vehicle__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__vehicle__free_unpacked
                     (Main__Vehicle *message,
                      ProtobufCAllocator *allocator);
/* Main__Balloon methods */
void   main__balloon__init
                     (Main__Balloon         *message);
size_t main__balloon__get_packed_size
                     (const Main__Balloon   *message);
size_t main__balloon__pack
                     (const Main__Balloon   *message,
                      uint8_t             *out);
size_t main__balloon__pack_to_buffer
                     (const Main__Balloon   *message,
                      ProtobufCBuffer     *buffer);
Main__Balloon *
       main__balloon__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__balloon__free_unpacked
                     (Main__Balloon *message,
                      ProtobufCAllocator *allocator);
/* Main__Report methods */
void   main__report__init
                     (Main__Report         *message);
size_t main__report__get_packed_size
                     (const Main__Report   *message);
size_t main__report__pack
                     (const Main__Report   *message,
                      uint8_t             *out);
size_t main__report__pack_to_buffer
                     (const Main__Report   *message,
                      ProtobufCBuffer     *buffer);
Main__Report *
       main__report__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__report__free_unpacked
                     (Main__Report *message,
                      ProtobufCAllocator *allocator);
/* Main__Spacecraft methods */
void   main__spacecraft__init
                     (Main__Spacecraft         *message);
size_t main__spacecraft__get_packed_size
                     (const Main__Spacecraft   *message);
size_t main__spacecraft__pack
                     (const Main__Spacecraft   *message,
                      uint8_t             *out);
size_t main__spacecraft__pack_to_buffer
                     (const Main__Spacecraft   *message,
                      ProtobufCBuffer     *buffer);
Main__Spacecraft *
       main__spacecraft__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__spacecraft__free_unpacked
                     (Main__Spacecraft *message,
                      ProtobufCAllocator *allocator);
/* Main__Weather methods */
void   main__weather__init
                     (Main__Weather         *message);
size_t main__weather__get_packed_size
                     (const Main__Weather   *message);
size_t main__weather__pack
                     (const Main__Weather   *message,
                      uint8_t             *out);
size_t main__weather__pack_to_buffer
                     (const Main__Weather   *message,
                      ProtobufCBuffer     *buffer);
Main__Weather *
       main__weather__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__weather__free_unpacked
                     (Main__Weather *message,
                      ProtobufCAllocator *allocator);
/* Main__Message methods */
void   main__message__init
                     (Main__Message         *message);
size_t main__message__get_packed_size
                     (const Main__Message   *message);
size_t main__message__pack
                     (const Main__Message   *message,
                      uint8_t             *out);
size_t main__message__pack_to_buffer
                     (const Main__Message   *message,
                      ProtobufCBuffer     *buffer);
Main__Message *
       main__message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__message__free_unpacked
                     (Main__Message *message,
                      ProtobufCAllocator *allocator);
/* Main__Audio methods */
void   main__audio__init
                     (Main__Audio         *message);
size_t main__audio__get_packed_size
                     (const Main__Audio   *message);
size_t main__audio__pack
                     (const Main__Audio   *message,
                      uint8_t             *out);
size_t main__audio__pack_to_buffer
                     (const Main__Audio   *message,
                      ProtobufCBuffer     *buffer);
Main__Audio *
       main__audio__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__audio__free_unpacked
                     (Main__Audio *message,
                      ProtobufCAllocator *allocator);
/* Main__File methods */
void   main__file__init
                     (Main__File         *message);
size_t main__file__get_packed_size
                     (const Main__File   *message);
size_t main__file__pack
                     (const Main__File   *message,
                      uint8_t             *out);
size_t main__file__pack_to_buffer
                     (const Main__File   *message,
                      ProtobufCBuffer     *buffer);
Main__File *
       main__file__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   main__file__free_unpacked
                     (Main__File *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Main__Location_Closure)
                 (const Main__Location *message,
                  void *closure_data);
typedef void (*Main__Payload_Closure)
                 (const Main__Payload *message,
                  void *closure_data);
typedef void (*Main__Request_Closure)
                 (const Main__Request *message,
                  void *closure_data);
typedef void (*Main__Pod_Closure)
                 (const Main__Pod *message,
                  void *closure_data);
typedef void (*Main__Data_Closure)
                 (const Main__Data *message,
                  void *closure_data);
typedef void (*Main__Plane_Closure)
                 (const Main__Plane *message,
                  void *closure_data);
typedef void (*Main__Vessel_Closure)
                 (const Main__Vessel *message,
                  void *closure_data);
typedef void (*Main__Vehicle_Closure)
                 (const Main__Vehicle *message,
                  void *closure_data);
typedef void (*Main__Balloon_Closure)
                 (const Main__Balloon *message,
                  void *closure_data);
typedef void (*Main__Report_Closure)
                 (const Main__Report *message,
                  void *closure_data);
typedef void (*Main__Spacecraft_Closure)
                 (const Main__Spacecraft *message,
                  void *closure_data);
typedef void (*Main__Weather_Closure)
                 (const Main__Weather *message,
                  void *closure_data);
typedef void (*Main__Message_Closure)
                 (const Main__Message *message,
                  void *closure_data);
typedef void (*Main__Audio_Closure)
                 (const Main__Audio *message,
                  void *closure_data);
typedef void (*Main__File_Closure)
                 (const Main__File *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor main__location__descriptor;
extern const ProtobufCMessageDescriptor main__payload__descriptor;
extern const ProtobufCMessageDescriptor main__request__descriptor;
extern const ProtobufCMessageDescriptor main__pod__descriptor;
extern const ProtobufCMessageDescriptor main__data__descriptor;
extern const ProtobufCMessageDescriptor main__plane__descriptor;
extern const ProtobufCMessageDescriptor main__vessel__descriptor;
extern const ProtobufCMessageDescriptor main__vehicle__descriptor;
extern const ProtobufCMessageDescriptor main__balloon__descriptor;
extern const ProtobufCMessageDescriptor main__report__descriptor;
extern const ProtobufCMessageDescriptor main__spacecraft__descriptor;
extern const ProtobufCMessageDescriptor main__weather__descriptor;
extern const ProtobufCMessageDescriptor main__message__descriptor;
extern const ProtobufCMessageDescriptor main__audio__descriptor;
extern const ProtobufCMessageDescriptor main__file__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_aulora_2eproto__INCLUDED */
