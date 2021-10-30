#ifndef GM6020_H
#define GM6020_H

#include "definition.h"

#define NOW 1
#define LAST 0

typedef struct{
    float KP;											//PID参数P
    float KI;											//PID参数I
    float KD;											//PID参数D
    float fdb;											//PID反馈值
    float ref;											//PID目标值
    float cur_error;									//当前误差
    float error[2];										//前两次误差
    float output;										//输出值
    float outputMax;									//最大输出值的绝对值

}PID_t;

typedef struct
{

    struct{
        int     msg_cnt;								// 收到消息数
        float   RotorAngle_0_360;						// 电机转子角度 单位 度° 范围0~360
        float   rpm;									// 电机转子速度，单位 rpm
        float   current;								// 电机转矩电流
    }FdbData;

    // struct{
    //     float     rpm_calc;							    // 机械转子角度差分出的rpm
    //     float     last_RotorAngle_0_360;                // 上次反馈的角度 单位度 0~360
    //     int       last_tick;                            // 上一次差分速度的时间
    // }Calculate;

     struct{
        float     RotorAngle_all;						// 电机总角度 单位 度° 范围-inf~inf
        float     RotorAngle_0_360_Log[2];				// NOW->电机当前位置0~360 LAST->电机上一次更新位置0~360
        float     RotorAngle_0_360_OffSet;				// 电机机械角度偏移量 单位 度°
        int       RotorRound;							// 电机转的圈数
    }Calculate;

    struct{    
        float     AxisAngle_inDegree;					// 电机轴输出角度 单位 度°
        float     AxisVelocity;							// 电机轴输出速度 单位 rpm
    }AxisData;

    
    float reductionRate;								// 减速比
    float encoder_resolution;							// 编码器分辨率

    PID_t speedPID;
    PID_t posPID;
}GM6020_t;

extern GM6020_t h6020s[7];

void CanTransmit_GM6020_1234(CAN_HandleTypeDef *hcanx, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq);
void CanTransmit_GM6020_567(CAN_HandleTypeDef *hcanx, int16_t cm5_iq, int16_t cm6_iq, int16_t cm7_iq);
void GM6020_Init(void);
void GM6020_ctrl(void);
void GM6020_Update(int id, uint8_t *fdbData);
HAL_StatusTypeDef GM6020_CanMsgDecode(uint32_t stdid,uint8_t *fdbData);
#endif
