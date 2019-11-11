/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: simple.proto */

#ifndef PROTOBUF_C_simple_2eproto__INCLUDED
#define PROTOBUF_C_simple_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Ap Ap;
typedef struct _SensorData SensorData;
typedef struct _SensorSet SensorSet;
typedef struct _PwmLed PwmLed;
typedef struct _ToSt ToSt;
typedef struct _ToHi ToHi;
typedef struct _ToHost ToHost;


/* --- enums --- */

typedef enum _StInfo {
  ST_INFO__STATUS = 1,
  ST_INFO__TOF = 2,
  ST_INFO__ACC = 4,
  ST_INFO__GYRO = 8,
  ST_INFO__IMU = 16,
  ST_INFO__HOP = 32,
  ST_INFO__MOTOR = 64,
  ST_INFO__ENC = 128,
  ST_INFO__BATTERY = 256
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(ST_INFO)
} StInfo;

/* --- messages --- */

struct  _Ap
{
  ProtobufCMessage base;
  char *ssid;
  char *ssid_type;
};
#define AP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&ap__descriptor) \
    , NULL, NULL }


struct  _SensorData
{
  ProtobufCMessage base;
  char *sensor_mac_address;
  protobuf_c_boolean has_sensor_type;
  uint32_t sensor_type;
  protobuf_c_boolean has_sensor_temperature;
  float sensor_temperature;
  protobuf_c_boolean has_sensor_humidity;
  float sensor_humidity;
  protobuf_c_boolean has_eco2_ppm;
  uint32_t eco2_ppm;
  protobuf_c_boolean has_tvoc_ppb;
  uint32_t tvoc_ppb;
  protobuf_c_boolean has_hall_state;
  protobuf_c_boolean hall_state;
  protobuf_c_boolean has_hall_interrupt;
  protobuf_c_boolean hall_interrupt;
  protobuf_c_boolean has_pir_interrupt;
  protobuf_c_boolean pir_interrupt;
  protobuf_c_boolean has_sensor_voltage;
  uint32_t sensor_voltage;
};
#define SENSOR_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sensor_data__descriptor) \
    , NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _SensorSet
{
  ProtobufCMessage base;
  char *sensor_mac_address;
  protobuf_c_boolean has_sensor_type;
  uint32_t sensor_type;
  protobuf_c_boolean has_interrupt_alarm;
  protobuf_c_boolean interrupt_alarm;
  protobuf_c_boolean has_notify_period;
  uint32_t notify_period;
  protobuf_c_boolean has_period_alarm;
  protobuf_c_boolean period_alarm;
  protobuf_c_boolean has_upper_threshold;
  uint32_t upper_threshold;
  protobuf_c_boolean has_lower_threshold;
  uint32_t lower_threshold;
};
#define SENSOR_SET__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sensor_set__descriptor) \
    , NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _PwmLed
{
  ProtobufCMessage base;
  protobuf_c_boolean has_pwm;
  uint32_t pwm;
  protobuf_c_boolean has_duty;
  uint32_t duty;
  protobuf_c_boolean has_period;
  uint32_t period;
  protobuf_c_boolean has_enable;
  uint32_t enable;
};
#define PWM_LED__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&pwm_led__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _ToSt
{
  ProtobufCMessage base;
  /*
   * {{{{{{{{{{{{{{{{{{{ from HI begin
   */
  /*
   * -350 ~ 350
   */
  protobuf_c_boolean has_l_speed;
  int32_t l_speed;
  /*
   * -350 ~ 350
   */
  protobuf_c_boolean has_r_speed;
  int32_t r_speed;
  /*
   * -350 ~ 350
   */
  protobuf_c_boolean has_neck_speed;
  uint32_t neck_speed;
  /*
   * st 에게 요청하는 정보, StInfo bitwise 연산
   */
  protobuf_c_boolean has_request_st_info;
  uint32_t request_st_info;
  SensorSet *sensor_set;
  protobuf_c_boolean has_test_1;
  uint32_t test_1;
  protobuf_c_boolean has_test_2;
  uint32_t test_2;
  protobuf_c_boolean has_test_3;
  uint32_t test_3;
  protobuf_c_boolean has_test_4;
  uint32_t test_4;
  protobuf_c_boolean has_test_5;
  uint32_t test_5;
  protobuf_c_boolean has_test_6;
  uint32_t test_6;
  protobuf_c_boolean has_test_7;
  uint32_t test_7;
  protobuf_c_boolean has_test_8;
  uint32_t test_8;
  protobuf_c_boolean has_test_9;
  uint32_t test_9;
  /*
   * from HI }}}}}}}}}}}}}}}}}}} 
   */
  protobuf_c_boolean has_test_10;
  uint32_t test_10;
};
#define TO_ST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&to_st__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _ToHi
{
  ProtobufCMessage base;
  ToHost *tohost_bypass;
  ToSt *tost_bypass;
  /*
   * {{{{{{{{{{{{{{{{{{{ from ST
   */
  protobuf_c_boolean has_tof;
  uint32_t tof;
  protobuf_c_boolean has_accx;
  float accx;
  protobuf_c_boolean has_accy;
  float accy;
  protobuf_c_boolean has_accz;
  float accz;
  protobuf_c_boolean has_gyro_x;
  float gyro_x;
  protobuf_c_boolean has_gyro_y;
  float gyro_y;
  protobuf_c_boolean has_gyro_z;
  float gyro_z;
  /*
   *rad
   */
  protobuf_c_boolean has_yaw;
  float yaw;
  /*
   *rad
   */
  protobuf_c_boolean has_pitch;
  float pitch;
  /*
   *rad
   */
  protobuf_c_boolean has_roll;
  float roll;
  /*
   * voltage
   */
  protobuf_c_boolean has_battery;
  float battery;
  /*
   *left motor rpm
   */
  protobuf_c_boolean has_lenc;
  int32_t lenc;
  /*
   *right motor rpm
   */
  protobuf_c_boolean has_renc;
  int32_t renc;
  protobuf_c_boolean has_power_button_pressed;
  protobuf_c_boolean power_button_pressed;
  /*
   * from ST }}}}}}}}}}}}}}}}}}} 
   */
  protobuf_c_boolean has_usb_con;
  protobuf_c_boolean usb_con;
  /*
   * {{{{{{{{{{{{{{{{{{{ from host
   */
  char *phone_public_key;
  protobuf_c_boolean has_do_charge;
  uint32_t do_charge;
  protobuf_c_boolean has_video_seq;
  uint32_t video_seq;
  protobuf_c_boolean has_set_mute;
  int32_t set_mute;
  protobuf_c_boolean has_audio;
  ProtobufCBinaryData audio;
  protobuf_c_boolean has_set_ir_cut1;
  uint32_t set_ir_cut1;
  /*
   * from host }}}}}}}}}}}}}}}}}}} 
   */
  protobuf_c_boolean has_set_ir_cut2;
  uint32_t set_ir_cut2;
  /*
   * {{{{{{{{{{{{{{{{{{{ from everyone
   */
  /*
   * from everyone }}}}}}}}}}}}}}}}}}}
   */
  PwmLed *pwm_led;
  protobuf_c_boolean has_test_1;
  uint32_t test_1;
  protobuf_c_boolean has_test_2;
  uint32_t test_2;
  protobuf_c_boolean has_test_3;
  uint32_t test_3;
  protobuf_c_boolean has_test_4;
  uint32_t test_4;
  protobuf_c_boolean has_test_5;
  uint32_t test_5;
  protobuf_c_boolean has_test_6;
  uint32_t test_6;
  protobuf_c_boolean has_test_7;
  uint32_t test_7;
  protobuf_c_boolean has_test_8;
  uint32_t test_8;
  protobuf_c_boolean has_test_9;
  uint32_t test_9;
  protobuf_c_boolean has_test_10;
  uint32_t test_10;
};
#define TO_HI__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&to_hi__descriptor) \
    , NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, {0,NULL}, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _ToHost
{
  ProtobufCMessage base;
  /*
   * {{{{{{{{{{{{{{{{{{{ from HI
   */
  /*
   * host 가 재생할 비디오
   */
  protobuf_c_boolean has_video;
  ProtobufCBinaryData video;
  /*
   * host 가 재생할 오디오
   */
  protobuf_c_boolean has_audio;
  ProtobufCBinaryData audio;
  /*
   * {{{{{{{{{{{{{{{{{{{ from Sensor WB55
   */
  /*
   * from Sensor WB55 }}}}}}}}}}}}}}}}}}}
   */
  SensorData *sensor_data;
  protobuf_c_boolean has_test_1;
  uint32_t test_1;
  protobuf_c_boolean has_test_2;
  uint32_t test_2;
  protobuf_c_boolean has_test_3;
  uint32_t test_3;
  protobuf_c_boolean has_test_4;
  uint32_t test_4;
  protobuf_c_boolean has_test_5;
  uint32_t test_5;
  protobuf_c_boolean has_test_6;
  uint32_t test_6;
  protobuf_c_boolean has_test_7;
  uint32_t test_7;
  protobuf_c_boolean has_test_8;
  uint32_t test_8;
  protobuf_c_boolean has_test_9;
  uint32_t test_9;
  protobuf_c_boolean has_test_10;
  uint32_t test_10;
};
#define TO_HOST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&to_host__descriptor) \
    , 0, {0,NULL}, 0, {0,NULL}, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


/* Ap methods */
void   ap__init
                     (Ap         *message);
size_t ap__get_packed_size
                     (const Ap   *message);
size_t ap__pack
                     (const Ap   *message,
                      uint8_t             *out);
size_t ap__pack_to_buffer
                     (const Ap   *message,
                      ProtobufCBuffer     *buffer);
Ap *
       ap__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   ap__free_unpacked
                     (Ap *message,
                      ProtobufCAllocator *allocator);
/* SensorData methods */
void   sensor_data__init
                     (SensorData         *message);
size_t sensor_data__get_packed_size
                     (const SensorData   *message);
size_t sensor_data__pack
                     (const SensorData   *message,
                      uint8_t             *out);
size_t sensor_data__pack_to_buffer
                     (const SensorData   *message,
                      ProtobufCBuffer     *buffer);
SensorData *
       sensor_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sensor_data__free_unpacked
                     (SensorData *message,
                      ProtobufCAllocator *allocator);
/* SensorSet methods */
void   sensor_set__init
                     (SensorSet         *message);
size_t sensor_set__get_packed_size
                     (const SensorSet   *message);
size_t sensor_set__pack
                     (const SensorSet   *message,
                      uint8_t             *out);
size_t sensor_set__pack_to_buffer
                     (const SensorSet   *message,
                      ProtobufCBuffer     *buffer);
SensorSet *
       sensor_set__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sensor_set__free_unpacked
                     (SensorSet *message,
                      ProtobufCAllocator *allocator);
/* PwmLed methods */
void   pwm_led__init
                     (PwmLed         *message);
size_t pwm_led__get_packed_size
                     (const PwmLed   *message);
size_t pwm_led__pack
                     (const PwmLed   *message,
                      uint8_t             *out);
size_t pwm_led__pack_to_buffer
                     (const PwmLed   *message,
                      ProtobufCBuffer     *buffer);
PwmLed *
       pwm_led__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   pwm_led__free_unpacked
                     (PwmLed *message,
                      ProtobufCAllocator *allocator);
/* ToSt methods */
void   to_st__init
                     (ToSt         *message);
size_t to_st__get_packed_size
                     (const ToSt   *message);
size_t to_st__pack
                     (const ToSt   *message,
                      uint8_t             *out);
size_t to_st__pack_to_buffer
                     (const ToSt   *message,
                      ProtobufCBuffer     *buffer);
ToSt *
       to_st__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   to_st__free_unpacked
                     (ToSt *message,
                      ProtobufCAllocator *allocator);
/* ToHi methods */
void   to_hi__init
                     (ToHi         *message);
size_t to_hi__get_packed_size
                     (const ToHi   *message);
size_t to_hi__pack
                     (const ToHi   *message,
                      uint8_t             *out);
size_t to_hi__pack_to_buffer
                     (const ToHi   *message,
                      ProtobufCBuffer     *buffer);
ToHi *
       to_hi__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   to_hi__free_unpacked
                     (ToHi *message,
                      ProtobufCAllocator *allocator);
/* ToHost methods */
void   to_host__init
                     (ToHost         *message);
size_t to_host__get_packed_size
                     (const ToHost   *message);
size_t to_host__pack
                     (const ToHost   *message,
                      uint8_t             *out);
size_t to_host__pack_to_buffer
                     (const ToHost   *message,
                      ProtobufCBuffer     *buffer);
ToHost *
       to_host__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   to_host__free_unpacked
                     (ToHost *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Ap_Closure)
                 (const Ap *message,
                  void *closure_data);
typedef void (*SensorData_Closure)
                 (const SensorData *message,
                  void *closure_data);
typedef void (*SensorSet_Closure)
                 (const SensorSet *message,
                  void *closure_data);
typedef void (*PwmLed_Closure)
                 (const PwmLed *message,
                  void *closure_data);
typedef void (*ToSt_Closure)
                 (const ToSt *message,
                  void *closure_data);
typedef void (*ToHi_Closure)
                 (const ToHi *message,
                  void *closure_data);
typedef void (*ToHost_Closure)
                 (const ToHost *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    st_info__descriptor;
extern const ProtobufCMessageDescriptor ap__descriptor;
extern const ProtobufCMessageDescriptor sensor_data__descriptor;
extern const ProtobufCMessageDescriptor sensor_set__descriptor;
extern const ProtobufCMessageDescriptor pwm_led__descriptor;
extern const ProtobufCMessageDescriptor to_st__descriptor;
extern const ProtobufCMessageDescriptor to_hi__descriptor;
extern const ProtobufCMessageDescriptor to_host__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_simple_2eproto__INCLUDED */
