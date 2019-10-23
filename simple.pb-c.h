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


typedef struct _SensorData SensorData;
typedef struct _SensorSet SensorSet;
typedef struct _RoughMotorControl RoughMotorControl;
typedef struct _LedControl LedControl;
typedef struct _PwmLed PwmLed;
typedef struct _EncoderControl EncoderControl;
typedef struct _ToBand ToBand;
typedef struct _ToWb55 ToWb55;
typedef struct _ToSt ToSt;
typedef struct _ReqCompleteSetup ReqCompleteSetup;
typedef struct _Ap Ap;
typedef struct _ToHi ToHi;
typedef struct _RspServiceKey RspServiceKey;
typedef struct _ToHost ToHost;


/* --- enums --- */

typedef enum _StInfo {
  /*
   *0x01
   */
  ST_INFO__STATUS = 1,
  /*
   *0x02
   */
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


struct  _RoughMotorControl
{
  ProtobufCMessage base;
  /*
   *0~60rpm
   */
  protobuf_c_boolean has_left_motor;
  int32_t left_motor;
  /*
   *0~60rpm
   */
  protobuf_c_boolean has_right_motor;
  int32_t right_motor;
};
#define ROUGH_MOTOR_CONTROL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rough_motor_control__descriptor) \
    , 0, 0, 0, 0 }


struct  _LedControl
{
  ProtobufCMessage base;
  protobuf_c_boolean has_left;
  uint32_t left;
  protobuf_c_boolean has_right;
  uint32_t right;
  protobuf_c_boolean has_bright;
  uint32_t bright;
};
#define LED_CONTROL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&led_control__descriptor) \
    , 0, 0, 0, 0, 0, 0 }


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


struct  _EncoderControl
{
  ProtobufCMessage base;
  protobuf_c_boolean has_left;
  uint32_t left;
  protobuf_c_boolean has_right;
  uint32_t right;
};
#define ENCODER_CONTROL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&encoder_control__descriptor) \
    , 0, 0, 0, 0 }


struct  _ToBand
{
  ProtobufCMessage base;
  /*
   * {{{{{{{{{{{{{{{{{{{ from ??
   */
  protobuf_c_boolean has_advertising_interval;
  uint32_t advertising_interval;
  protobuf_c_boolean has_tx_power;
  uint32_t tx_power;
  char *tag_name;
  /*
   * 
   */
  char *tag_phone_number;
  /*
   * 
   */
  protobuf_c_boolean has_lcd_angle;
  uint32_t lcd_angle;
  /*
   * 
   */
  protobuf_c_boolean has_lcd_time;
  uint32_t lcd_time;
  /*
   * 
   */
  protobuf_c_boolean has_data_hz;
  uint32_t data_hz;
  /*
   * 
   */
  protobuf_c_boolean has_data_req;
  protobuf_c_boolean data_req;
  /*
   * hi ¿¡¼­ serial resender ¸ñÀûÀ¸·Î seq ¸¦ Ã¤¿ö¼­ ÁØ´Ù. ¸¸¾à ÀÌ °ªÀÌ ÀÖ´Ù¸é, wb55 ´Â seq ¸¦ ±×´ë·Î ´ã¾Æ¼­ º¸³»¾ß ÇÑ´Ù.
   */
  protobuf_c_boolean has_seq;
  uint32_t seq;
};
#define TO_BAND__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&to_band__descriptor) \
    , 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _ToWb55
{
  ProtobufCMessage base;
  /*
   * {{{{{{{{{{{{{{{{{{{ from ??
   */
  protobuf_c_boolean has_broadcast_interval;
  uint32_t broadcast_interval;
  protobuf_c_boolean has_power;
  uint32_t power;
  char *name;
  /*
   * 
   */
  protobuf_c_boolean has_get_st_message;
  protobuf_c_boolean get_st_message;
  /*
   * hi 에서 serial resender 목적으로 seq 를 채워서 준다. 만약 이 값이 있다면, wb55 는 seq 를 그대로 담아서 보내야 한다.
   */
  protobuf_c_boolean has_seq;
  uint32_t seq;
  /*
   * android, st 에서 Hi 로 줄 데이터는 여기에 넣는다. 
   * wb55 가 get_st_message 가 들어왔을 때 만들어놓은 serilize bytes 가 있다면 패킷을 만들어서 전송한다. 
   */
  ToHi *to_hi_from_host;
  /*
   * ble data 
   */
  ToHost *to_host_from_hi;
  /*
   * ble data 
   */
  ToSt *to_st_from_hi;
  ToHi *to_hi_from_band;
  ToSt *to_st_from_host;
  /*
   * {{{{{{{{{{{{{{{{{{{ from Host
   */
  /*
   * from Host }}}}}}}}}}}}}}}}}}}
   */
  SensorSet *sensor_set;
};
#define TO_WB55__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&to_wb55__descriptor) \
    , 0, 0, 0, 0, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL }


struct  _ToSt
{
  ProtobufCMessage base;
  /*
   * {{{{{{{{{{{{{{{{{{{ from HI begin
   */
  RoughMotorControl *rough_motor_control;
  EncoderControl *encoder_control;
  LedControl *led_control;
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
  /*
   * 전원 명령, 1 이면 꺼라. 2이면 슬립모드... 등등
   */
  protobuf_c_boolean has_power_command;
  uint32_t power_command;
  /*
   * 충전해라.
   */
  protobuf_c_boolean has_do_charge;
  uint32_t do_charge;
  /*
   * 1 이면 시리얼이 특정 시간만큼 안들어왔을 때 전원 리셋
   */
  protobuf_c_boolean has_reset_if_no_serial;
  uint32_t reset_if_no_serial;
  /*
   * 여기에 속한 데이터를 그대로 담아서 응답해야 함.
   */
  protobuf_c_boolean has_loopback;
  ProtobufCBinaryData loopback;
  SensorSet *sensor_set;
  /*
   * /&lt; 증가하는값인데, 이미 처리된 명령이라면 기억하고 있는 마지막 응답을 돌려줘야 한다.  
   */
  uint32_t seq;
};
#define TO_ST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&to_st__descriptor) \
    , NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0,NULL}, NULL, 0 }


struct  _ReqCompleteSetup
{
  ProtobufCMessage base;
  char *user_id;
  char *device_id;
};
#define REQ_COMPLETE_SETUP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&req_complete_setup__descriptor) \
    , NULL, NULL }


struct  _Ap
{
  ProtobufCMessage base;
  char *ssid;
  char *ssid_type;
};
#define AP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&ap__descriptor) \
    , NULL, NULL }


struct  _ToHi
{
  ProtobufCMessage base;
  ToHost *tohost_bypass;
  ToSt *tost_bypass;
  /*
   * {{{{{{{{{{{{{{{{{{{ from ST
   */
  protobuf_c_boolean has_status;
  uint32_t status;
  /*
   *mm
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
   * neck position
   */
  protobuf_c_boolean has_hopadc;
  uint32_t hopadc;
  /*
   *-2500~2500
   */
  protobuf_c_boolean has_lspeed;
  int32_t lspeed;
  /*
   *-2500~2500
   */
  protobuf_c_boolean has_rspeed;
  int32_t rspeed;
  /*
   *-2500~2500   neck speed
   */
  protobuf_c_boolean has_t_speed;
  int32_t t_speed;
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
  protobuf_c_boolean has_usb_con;
  protobuf_c_boolean usb_con;
  protobuf_c_boolean has_charge_complete;
  protobuf_c_boolean charge_complete;
  protobuf_c_boolean has_auto_charge_step;
  uint32_t auto_charge_step;
  /*
   * ToSt::loopback 으로 전달된 데이터가 다시 받아지는 것.  
   */
  protobuf_c_boolean has_loopback;
  ProtobufCBinaryData loopback;
  /*
   * {{{{{{{{{{{{{{{{{{{ from WB55 Client, Dongle
   */
  /*
   *sec
   */
  char *mac_address;
  /*
   *sec
   */
  char *dev_name;
  /*
   *sec
   */
  protobuf_c_boolean has_dev_rssi;
  uint32_t dev_rssi;
  /*
   *sec
   */
  protobuf_c_boolean has_connected;
  protobuf_c_boolean connected;
  char *connected_mac;
  /*
   *sec
   */
  protobuf_c_boolean has_disconnected;
  protobuf_c_boolean disconnected;
  /*
   *optional VL53L1Data vl53l1_data = 57;
   * from WB55 }}}}}}}}}}}}}}}}}}} 
   */
  char *disconnected_mac;
  /*
   * {{{{{{{{{{{{{{{{{{{ from host
   */
  char *phone_public_key;
  protobuf_c_boolean has_do_charge;
  uint32_t do_charge;
  protobuf_c_boolean has_video_seq;
  uint32_t video_seq;
  protobuf_c_boolean has_set_ir_cut1;
  uint32_t set_ir_cut1;
  protobuf_c_boolean has_set_ir_cut2;
  uint32_t set_ir_cut2;
  protobuf_c_boolean has_set_mute;
  int32_t set_mute;
  /*
   * from host }}}}}}}}}}}}}}}}}}} 
   */
  protobuf_c_boolean has_audio;
  ProtobufCBinaryData audio;
  /*
   * {{{{{{{{{{{{{{{{{{{ from BLE
   */
  char *ssid;
  char *ssid_password;
  char *ssid_type;
  protobuf_c_boolean has_req_service_key;
  uint32_t req_service_key;
  ReqCompleteSetup *req_complete_setup;
  /*
   * HI 한테 주변 AP 검색하라고 명령.
   */
  protobuf_c_boolean has_req_scan_ap;
  uint32_t req_scan_ap;
  protobuf_c_boolean has_seq;
  uint32_t seq;
  /*
   * from BLE }}}}}}}}}}}}}}}}}}} 
   */
  char *sender;
  /*
   * {{{{{{{{{{{{{{{{{{{ from everyone
   */
  /*
   * from everyone }}}}}}}}}}}}}}}}}}} 
   */
  PwmLed *pwm_led;
};
#define TO_HI__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&to_hi__descriptor) \
    , NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0,NULL}, NULL, NULL, 0, 0, 0, 0, NULL, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0,NULL}, NULL, NULL, NULL, 0, 0, NULL, 0, 0, 0, 0, NULL, NULL }


struct  _RspServiceKey
{
  ProtobufCMessage base;
  char *service_key;
  char *mac_address;
  char *model_name;
  char *serial_number;
};
#define RSP_SERVICE_KEY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rsp_service_key__descriptor) \
    , NULL, NULL, NULL, NULL }


struct  _ToHost
{
  ProtobufCMessage base;
  /*
   * {{{{{{{{{{{{{{{{{{{ from HI
   */
  /*
   * host 가 재생할 비디오(jpeg)
   */
  protobuf_c_boolean has_video;
  ProtobufCBinaryData video;
  /*
   * host 가 재생할 오디오 
   */
  protobuf_c_boolean has_audio;
  ProtobufCBinaryData audio;
  /*
   * from HI }}}}}}}}}}}}}}}}}}} 
   */
  ToHi *tohi_bypass;
  /*
   * {{{{{{{{{{{{{{{{{{{ from BLE    
   */
  /*
   * ReqServiceKey 의 응답
   */
  RspServiceKey *rsp_service_key;
  /*
   * ReqCompleteSetup 의 응답 
   */
  protobuf_c_boolean has_rsp_complete_setup;
  uint32_t rsp_complete_setup;
  /*
   * ap scan 후 돌려주는 리스트, 폰에서 이 중 하나를 선택함.  
   */
  size_t n_ap_list;
  Ap **ap_list;
  /*
   * ap 접속시도 결과 리턴   
   */
  protobuf_c_boolean has_ap_connection_result;
  int32_t ap_connection_result;
  /*
   * {{{{{{{{{{{{{{{{{{{ from Sensor WB55
   */
  /*
   * from Sensor WB55 }}}}}}}}}}}}}}}}}}} 
   */
  SensorData *sensor_data;
};
#define TO_HOST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&to_host__descriptor) \
    , 0, {0,NULL}, 0, {0,NULL}, NULL, NULL, 0, 0, 0,NULL, 0, 0, NULL }


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
/* RoughMotorControl methods */
void   rough_motor_control__init
                     (RoughMotorControl         *message);
size_t rough_motor_control__get_packed_size
                     (const RoughMotorControl   *message);
size_t rough_motor_control__pack
                     (const RoughMotorControl   *message,
                      uint8_t             *out);
size_t rough_motor_control__pack_to_buffer
                     (const RoughMotorControl   *message,
                      ProtobufCBuffer     *buffer);
RoughMotorControl *
       rough_motor_control__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rough_motor_control__free_unpacked
                     (RoughMotorControl *message,
                      ProtobufCAllocator *allocator);
/* LedControl methods */
void   led_control__init
                     (LedControl         *message);
size_t led_control__get_packed_size
                     (const LedControl   *message);
size_t led_control__pack
                     (const LedControl   *message,
                      uint8_t             *out);
size_t led_control__pack_to_buffer
                     (const LedControl   *message,
                      ProtobufCBuffer     *buffer);
LedControl *
       led_control__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   led_control__free_unpacked
                     (LedControl *message,
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
/* EncoderControl methods */
void   encoder_control__init
                     (EncoderControl         *message);
size_t encoder_control__get_packed_size
                     (const EncoderControl   *message);
size_t encoder_control__pack
                     (const EncoderControl   *message,
                      uint8_t             *out);
size_t encoder_control__pack_to_buffer
                     (const EncoderControl   *message,
                      ProtobufCBuffer     *buffer);
EncoderControl *
       encoder_control__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   encoder_control__free_unpacked
                     (EncoderControl *message,
                      ProtobufCAllocator *allocator);
/* ToBand methods */
void   to_band__init
                     (ToBand         *message);
size_t to_band__get_packed_size
                     (const ToBand   *message);
size_t to_band__pack
                     (const ToBand   *message,
                      uint8_t             *out);
size_t to_band__pack_to_buffer
                     (const ToBand   *message,
                      ProtobufCBuffer     *buffer);
ToBand *
       to_band__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   to_band__free_unpacked
                     (ToBand *message,
                      ProtobufCAllocator *allocator);
/* ToWb55 methods */
void   to_wb55__init
                     (ToWb55         *message);
size_t to_wb55__get_packed_size
                     (const ToWb55   *message);
size_t to_wb55__pack
                     (const ToWb55   *message,
                      uint8_t             *out);
size_t to_wb55__pack_to_buffer
                     (const ToWb55   *message,
                      ProtobufCBuffer     *buffer);
ToWb55 *
       to_wb55__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   to_wb55__free_unpacked
                     (ToWb55 *message,
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
/* ReqCompleteSetup methods */
void   req_complete_setup__init
                     (ReqCompleteSetup         *message);
size_t req_complete_setup__get_packed_size
                     (const ReqCompleteSetup   *message);
size_t req_complete_setup__pack
                     (const ReqCompleteSetup   *message,
                      uint8_t             *out);
size_t req_complete_setup__pack_to_buffer
                     (const ReqCompleteSetup   *message,
                      ProtobufCBuffer     *buffer);
ReqCompleteSetup *
       req_complete_setup__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   req_complete_setup__free_unpacked
                     (ReqCompleteSetup *message,
                      ProtobufCAllocator *allocator);
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
/* RspServiceKey methods */
void   rsp_service_key__init
                     (RspServiceKey         *message);
size_t rsp_service_key__get_packed_size
                     (const RspServiceKey   *message);
size_t rsp_service_key__pack
                     (const RspServiceKey   *message,
                      uint8_t             *out);
size_t rsp_service_key__pack_to_buffer
                     (const RspServiceKey   *message,
                      ProtobufCBuffer     *buffer);
RspServiceKey *
       rsp_service_key__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rsp_service_key__free_unpacked
                     (RspServiceKey *message,
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

typedef void (*SensorData_Closure)
                 (const SensorData *message,
                  void *closure_data);
typedef void (*SensorSet_Closure)
                 (const SensorSet *message,
                  void *closure_data);
typedef void (*RoughMotorControl_Closure)
                 (const RoughMotorControl *message,
                  void *closure_data);
typedef void (*LedControl_Closure)
                 (const LedControl *message,
                  void *closure_data);
typedef void (*PwmLed_Closure)
                 (const PwmLed *message,
                  void *closure_data);
typedef void (*EncoderControl_Closure)
                 (const EncoderControl *message,
                  void *closure_data);
typedef void (*ToBand_Closure)
                 (const ToBand *message,
                  void *closure_data);
typedef void (*ToWb55_Closure)
                 (const ToWb55 *message,
                  void *closure_data);
typedef void (*ToSt_Closure)
                 (const ToSt *message,
                  void *closure_data);
typedef void (*ReqCompleteSetup_Closure)
                 (const ReqCompleteSetup *message,
                  void *closure_data);
typedef void (*Ap_Closure)
                 (const Ap *message,
                  void *closure_data);
typedef void (*ToHi_Closure)
                 (const ToHi *message,
                  void *closure_data);
typedef void (*RspServiceKey_Closure)
                 (const RspServiceKey *message,
                  void *closure_data);
typedef void (*ToHost_Closure)
                 (const ToHost *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    st_info__descriptor;
extern const ProtobufCMessageDescriptor sensor_data__descriptor;
extern const ProtobufCMessageDescriptor sensor_set__descriptor;
extern const ProtobufCMessageDescriptor rough_motor_control__descriptor;
extern const ProtobufCMessageDescriptor led_control__descriptor;
extern const ProtobufCMessageDescriptor pwm_led__descriptor;
extern const ProtobufCMessageDescriptor encoder_control__descriptor;
extern const ProtobufCMessageDescriptor to_band__descriptor;
extern const ProtobufCMessageDescriptor to_wb55__descriptor;
extern const ProtobufCMessageDescriptor to_st__descriptor;
extern const ProtobufCMessageDescriptor req_complete_setup__descriptor;
extern const ProtobufCMessageDescriptor ap__descriptor;
extern const ProtobufCMessageDescriptor to_hi__descriptor;
extern const ProtobufCMessageDescriptor rsp_service_key__descriptor;
extern const ProtobufCMessageDescriptor to_host__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_simple_2eproto__INCLUDED */
