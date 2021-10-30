//
// Created by zhuzhengming on 2021/10/28.
//
#include "mission.h"

//中断
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(hitm->Instance == TIM2){

        GM6020_ctrl();
    }
}

void angle_PID(PID_t *pid)
{
	pid->cur_error = pid->ref - pid->fdb;
	pid->output += pid->KP * (pid->cur_error - pid->error[1]) + pid->KI * pid->cur_error + pid->KD * (pid->cur_error - 2 * pid->error[1] + pid->error[0]);
	pid->error[0] = pid->error[1];
	pid->error[1] = pid->ref - pid->fdb;

	/*设定输出上限*/
	if(pid->output > pid->outputMax) pid->output = pid->outputMax;
	if(pid->output < -pid->outputMax) pid->output = -pid->outputMax;

}


void mode_change(void){
    switch(joy_revbag.button){
        
        //yaw = 0; pitch = 0
        case 0:
            h6020s[0].posPID.fdb = imu_revbag.eul[1];
            h6020s[1].posPID.fdb = imu_revbag.eul[2];

            h6020s[0].posPID.ref = 0;
            h6020s[1].posPID.ref = 0;
        break;

        //joystick controll
        case 1:

            h6020s[0].posPID.fdb = imu_revbag.eul[1];
            h6020s[1].posPID.fdb = imu_revbag.eul[2];

            h6020s[0].posPID.ref = (float)((joy_revbag.left_joysticks[0]-0.5) * 90.0f);
            h6020s[1].posPID.ref = (float)((joy_revbag.left_joysticks[1]-0.5) * 90.0f);

        break;

        //object_track
        case 2:

            break;

    }
}