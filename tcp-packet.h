#pragma once

namespace IP { 
    static const char* registerServer = "register.varram-robot.com";
    //static const char* registerServer = "kangssu.com";
    static const char* registerServerPort = "80"; 
    //static const char* registerServer = "192.168.103.59";
    //static const char* registerServerPort = "80"; 
}

#include <stdint.h>

typedef uint64_t u64;
typedef	uint32_t u32;
typedef	uint16_t u16;
typedef	signed short s16;
typedef uint8_t u8;
typedef signed char s8;

#define PKT_VIDEO_REQUEST		1
#define PKT_AUDIO_SEND		2
#define PKT_		2

#define PKT_TO_DEVICE(A, B) const int A = B
#define PKT_TO_MOBILE(A, B) const int A = B
#define PKT_TO_LICENSE(A, B) const int A = B

// balbbot2

enum {
    MEDIA_PORT = 13001,
    DATA_PORT = 13002 
};

enum {
    VIDEO_PACKET = 5,
    AUDIO_PACKET = 6,
    REQ_CONNECT = 48,
    RSP_CONNECT = 49,
    REQ_SERIAL_KEY = 50,
    RSP_SERIAL_KEY = 51,
    REQ_PRODUCT_LICENSE = 52,
    RSP_PRODUCT_LICENSE = 53,
    REQ_ACTIVATE_LICENSE = 54,
    RSP_ACTIVATE_LICENSE = 55,
    REQ_DEVICE_SETUP = 56,
    RSP_DEVICE_SETUP = 57,
    REQ_TOKEN_UPDATE = 58,
    RSP_TOKEN_UPDATE = 59, 
};

PKT_TO_DEVICE(REQ_CONTROLLER, 98);
PKT_TO_DEVICE(REQ_USERKEY, 100);
PKT_TO_MOBILE(RSP_USERKEY, 101);
PKT_TO_DEVICE(REQ_SCAN_WIFI, 102);
PKT_TO_MOBILE(RSP_SCAN_WIFI, 103);


PKT_TO_DEVICE(REQ_CONNECT_AP, 104);
PKT_TO_MOBILE(RSP_CONNECT_AP, 105);

PKT_TO_DEVICE(REQ_CONNECT_SERVER, 106);
PKT_TO_MOBILE(RSP_CONNECT_SERVER, 107); 


#pragma pack(push, 1) 
typedef struct 
{
	u16	cmdType;	// 명령의 종류
	u32	dataLen;	// 본 패킷의 총 전송 길이
    u8 data[0];
    //u8 data[0];
}PKT; 
typedef struct 
{
    u8 s1;
    u8 s2;
    u8 s3;
    u8 s4;
	u16	cmdType;	// 명령의 종류
	u32	dataLen;	// 본 패킷의 총 전송 길이
    u8 data[0];
    //u8 data[0];
}PKT_SIG; 
#pragma pack(pop)
int writeWithSegment(int fds, uint8_t* buf, int bufSize, int segmentSize, bool retry);
int readWithSegment(int fds, uint8_t* buf, int bufSize, int segmentSize, bool retry);
int writeWithPKT(int fds, const PKT& pkt, uint8_t* buf, int bufSize, bool retry);
int makeHeader(int cmdType, uint8_t* pkt, int bugSize);
