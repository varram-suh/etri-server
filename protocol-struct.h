
#pragma once

#include <inttypes.h>
#include <memory.h>
#include <cstdlib>
#pragma pack(push, 1) 
struct ModernProtocolWithSTM {
    uint8_t sig1;
    uint8_t sig2;
    uint8_t crc;
    uint32_t length;
    uint8_t data[0];
}; 
struct InnerProtocolWithSTM {
    uint8_t sig1;
    uint8_t sig2;
    uint8_t command;
    uint8_t length[3];
    uint8_t data[0];
}; 


enum FirmwareUpdateMode : uint8_t { 
    START = 0,
    END = 1,
    DATA = 2,
    VERIFY = 3,
};
struct FirmwareUpgradeSimple {
    uint8_t firmwareId;
    uint8_t updateMode;
};
struct FirmwareUpgradeDetail {
    uint8_t firmwareId;
    uint8_t updateMode;
    uint32_t startOffset;
    uint32_t endOffset;
    uint8_t data[0];
};
struct FirmwareUpgradeVerify {
    uint8_t firmwareId;
    uint8_t updateMode;
    uint32_t startOffset;
    uint32_t endOffset;
};
struct FirmwareUpgradeResponse {
    uint8_t firmwareId;
    uint8_t updateMode; 
    uint8_t status; 
    uint32_t crc32; 
};



struct ProtocolWithSTM {
    uint8_t sig1;
    uint8_t sig2;
    uint8_t command;
    uint8_t crc;
    uint16_t length;
    uint8_t data[0];
};

struct CONTROL_REMOTE{
    static const int value = 0x4A;
    int16_t left;
    int16_t right;
    uint16_t ADJ_INTERVAL;
    uint16_t ACCDEC;
    uint16_t ACC_ENABLE;
};

struct CONTROL_YAW_RESET {
    static const int value = 0x7A;
};

using ControlMotorSTM = CONTROL_REMOTE;

struct CONTROL_WHEEL{
    static const int value = 0x8A;
    int16_t LEFT;
    int16_t RIGHT;
    uint16_t IMU_USE;
};

struct ControlTossingSTM{
    int16_t motor;
};
struct CONTROL_ENCODER{
    int16_t LEFT;
    int16_t RIGHT;
    static const int value = 0x9A;
};


struct CONTROL_ST_AVOID {
    int32_t EXECUTE;
    enum {value=0xBA};
};

struct CONTROL_ABS_ROTATE {
    float DEGREE;
    int32_t DIRECTION;
    uint32_t SPEED;
    enum {value=0xCA};
};

struct CONTROL_REL_ROTATE {
    float DEGREE;
    uint32_t SPEED;
    enum {value=0xDA};
};

struct CONTROL_STRAIGHT {
    float DISTANCE_CM;
    uint32_t SPEED;
    enum {value=0xEA};
}; 
#pragma pack(pop)
