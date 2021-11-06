#ifndef DEFINITION_H
#define DEFINITION_H

#define PI 3.1415926
#define _SIGN(x) (x>0? 1: (x<0? -1 :0) )

#include "can.h"
#include "gpio.h"
#include "math.h"
#include "stdio.h"
#include "usart.h"
#include "tim.h"
#include "dma.h"


#include "can_app.h"
#include "GM6020.h"
#include "proj_init.h"
#include "app_serial.h"
#include "mission.h"

//joy
#define JOY_HEADER_HIGH 0xff
#define JOY_HEADER_LOW 0x55
#define JOY_ADDRESS_HIGH 0x00
#define JOY_ADDRESS_LOW 0x01
#define JOY_CHANNEL 0x14
#define WRROG 0x00
#define OK 0x01
#define JOY_HEAD_LENGTH 2
#define JOY_ADD_LENGTH 2
#define JOY_CHANNEL_LENGTH 1
#define JOY_PAYLOAD_LENGTH 5
#define JOY_BAG_LENGTH (JOY_HEAD_LENGTH + JOY_PAYLOAD_LENGTH + JOY_ADD_LENGTH + JOY_CHANNEL_LENGTH + 3)

//IMU
#define IMU_HEADER 0x5a
#define IMU_TYPE 0xa5
#define IMU_HEAD_LENGTH 6
#define IMU_PAYLOAD_LENGTH 76
#define IMU_BAG_LENGTH (IMU_HEAD_LENGTH+ IMU_PAYLOAD_LENGTH)

//YOLO
#define YOLO_HEADER_HIGH 0x11
#define YOLO_HEADER_LOW 0xa5
#define YOLO_ADDRESS_HIGH 0x00
#define YOLO_ADDRESS_LOW 0x03
#define YOLO_CHANNEL 0x16
#define YOLO_HEAD_LENGTH 2
#define YOLO_ADD_LENGTH 2
#define YOLO_PAYLOAD_LENGTH 17
#define YOLO_BAG_LENGTH (YOLO_HEAD_LENGTH+YOLO_ADD_LENGTH+YOLO_PAYLOAD_LENGTH+2)


typedef union {
    uint8_t raw[IMU_BAG_LENGTH ];
    struct {
        union {
            uint8_t head_data[IMU_HEAD_LENGTH];
            struct {
                uint8_t head;
                uint8_t type;
                uint16_t lentgh;
                uint16_t crc16;
            }__attribute__((packed));
        };
        union {
            uint8_t payload[IMU_PAYLOAD_LENGTH];
            struct {
                uint8_t     tag;
                uint8_t     id;
                uint8_t     rev[2];
                float       press;
                uint32_t    times;
                float   acc[3];
                float   gyr[3];
                float   mag[3];     
                float   eul[3];     //roll,pitch,Yaw
                float  quat[4];
            }__attribute__((packed));
        };
    } __attribute__((packed));
} __attribute__((packed)) IMU_Bag_t;


typedef union {
    uint8_t raw[YOLO_BAG_LENGTH];
    struct {
        uint8_t ADD[YOLO_ADD_LENGTH];
        uint8_t CHAN;
        uint8_t head[YOLO_HEAD_LENGTH];
        uint8_t length;
        union {
            uint8_t payload[YOLO_PAYLOAD_LENGTH];
            struct {
                int image_rows;
                int image_cols;
                float x;
                float y;
                uint8_t object_num;
            }__attribute__((packed));
        };
    } __attribute__((packed));
} __attribute__((packed)) YOLO_Bag_t;


typedef union {
    uint8_t raw[JOY_BAG_LENGTH];
    struct {
        uint8_t ADD[JOY_ADD_LENGTH];
        uint8_t CHAN[JOY_CHANNEL_LENGTH];
        uint8_t head[JOY_HEAD_LENGTH];
        uint8_t length;
        uint8_t crc8;
        uint8_t state;
        union {
            uint8_t payload[JOY_PAYLOAD_LENGTH];
            struct {
                uint8_t left_joysticks[2];
                uint8_t right_joysticks[2];
                uint8_t buttons;
            }__attribute__((packed));
        };
    } __attribute__((packed));
} __attribute__((packed)) Joy_Bag_t;


extern Joy_Bag_t joy_revbag;
extern YOLO_Bag_t yolo_revbag;
extern IMU_Bag_t imu_revbag;

extern uint8_t joy_raw_data[2 * JOY_BAG_LENGTH];
extern uint8_t yolo_raw_data[2*YOLO_BAG_LENGTH];
extern uint8_t imu_raw_data[2*IMU_BAG_LENGTH];


extern uint8_t flag_counter;

#endif