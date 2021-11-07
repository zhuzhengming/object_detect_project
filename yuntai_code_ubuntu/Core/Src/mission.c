//
// Created by zhuzhengming on 2021/10/28.
//
#include "mission.h"
#define K 10.0

//void pid_update_for_object_tracking(void){
//        for(int i =0; i<7; i++)
//        {
//
//            h6020s[i].speedPID.KP = 20.0;
//            h6020s[i].speedPID.KI = 0;
//            h6020s[i].speedPID.KD = 0;
//            h6020s[i].speedPID.outputMax = 6000;
//
//            h6020s[i].posPID.KP = 10.0f;
//            //h6020s[i].posPID.KP = 50.0f;
//            h6020s[i].posPID.KI = 0;
//            h6020s[i].posPID.KD =0;
//            h6020s[i].posPID.outputMax = 1500;
//
//            h6020s[i].reductionRate = 1.0f;
//            h6020s[i].encoder_resolution = 8192.0f;
//        }
//}

void mode_change(void){
    joy_revbag.buttons = 2;
    switch(joy_revbag.buttons){

        //yaw = 0; pitch = 0
        case 0:
            h6020s[1].posPID.fdb = imu_revbag.eul[0];   //pitch
            h6020s[1].posPID.ref = 0.0;

            //stable
            h6020s[0].posPID.fdb = h6020s[0].AxisData.AxisAngle_inDegree;
            h6020s[0].posPID.ref = 0.0;
            break;

            //joystick control
        case 1:

            h6020s[0].posPID.fdb = imu_revbag.eul[1];
            h6020s[1].posPID.fdb = imu_revbag.eul[2];

            h6020s[0].posPID.ref = (float)((joy_revbag.left_joysticks[0]-0.5) * 90.0f);
            h6020s[1].posPID.ref = (float)((joy_revbag.left_joysticks[1]-0.5) * 90.0f);

            break;

        //object_track
        case 2:
            //update pid
//            pid_update_for_object_tracking();

            if(yolo_revbag.object_num == 1 && yolo_revbag.x > 20.0 && yolo_revbag.y > 20.0) {

                h6020s[0].posPID.fdb = (float) ( yolo_revbag.x / K);
                h6020s[1].posPID.fdb = (float) (  yolo_revbag.y / K);
            }

            //center position
            h6020s[0].posPID.ref = (float )(Image_rows/(K*2));
            h6020s[1].posPID.ref = (float )(Image_cols/(K*2));

//            if(h6020s[0].posPID.ref - h6020s[0].posPID.fdb > 10.0)   h6020s[0].posPID.ref = h6020s[0].posPID.fdb + 10.0;
//            if(h6020s[0].posPID.ref - h6020s[0].posPID.fdb < -10.0)     h6020s[0].posPID.ref = h6020s[0].posPID.fdb - 10.0;
//            if(h6020s[1].posPID.ref - h6020s[1].posPID.fdb > 10.0)   h6020s[1].posPID.ref = h6020s[1].posPID.fdb + 10.0;
//            if(h6020s[1].posPID.ref - h6020s[1].posPID.fdb < -10.0)     h6020s[1].posPID.ref = h6020s[1].posPID.fdb - 10.0;

            break;

    }
}


//  100kHZ
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2){

        GM6020_ctrl();
        mode_change();
    }
}

//void angle_PID(PID_t *pid)
//{
//	pid->cur_error = pid->ref - pid->fdb;
//	pid->output += pid->KP * (pid->cur_error - pid->error[1]) + pid->KI * pid->cur_error + pid->KD * (pid->cur_error - 2 * pid->error[1] + pid->error[0]);
//	pid->error[0] = pid->error[1];
//	pid->error[1] = pid->ref - pid->fdb;
//
//	/*设定输出上限*/
//	if(pid->output > pid->outputMax) pid->output = pid->outputMax;
//	if(pid->output < -pid->outputMax) pid->output = -pid->outputMax;
//
//}


