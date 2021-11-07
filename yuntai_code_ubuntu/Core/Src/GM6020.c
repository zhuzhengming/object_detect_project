#include "GM6020.h"
#include "math.h"

GM6020_t h6020s[7];
float posRef = 60.0f;

void GM6020_Init(void) 
{
	for(int i =0; i<7; i++)
	 {

//		h6020s[i].speedPID.KP = 15;
//        h6020s[i].speedPID.KI = 1;
//        h6020s[i].speedPID.KD = 1;
//        h6020s[i].speedPID.outputMax = 20000;
//
//        h6020s[i].posPID.KP = 20.0f;
//        //h6020s[i].posPID.KP = 50.0f;
//        h6020s[i].posPID.KI = 0;
//        h6020s[i].posPID.KD =0;
//        h6020s[i].posPID.outputMax = 1500;
//
//		h6020s[i].reductionRate = 1.0f;
//		h6020s[i].encoder_resolution = 8192.0f;

//test pid for object tracking
            h6020s[i].speedPID.KP = 5.0;
            h6020s[i].speedPID.KI = 0.001;
            h6020s[i].speedPID.KD = 0.05;
            h6020s[i].speedPID.outputMax = 6000;

            h6020s[i].posPID.KP = 2.0f;
            h6020s[i].posPID.KI = 0;
            h6020s[i].posPID.KD =0;
            h6020s[i].posPID.outputMax = 1500;

            h6020s[i].reductionRate = 1.0f;
            h6020s[i].encoder_resolution = 8192.0f;

	}
}

static uint32_t TxMailbox;

void P_Calc(PID_t *pid){
	pid->cur_error = pid->ref - pid->fdb;
	pid->output = pid->KP * pid->cur_error;
	/*设定输出上限*/
	if(pid->output > pid->outputMax) pid->output = pid->outputMax;
	if(pid->output < -pid->outputMax) pid->output = -pid->outputMax;

    //reverse output due to camera image
//    if(joy_revbag.buttons == 2){
//        pid->cur_error = pid->ref - pid->fdb;
//        pid->output = pid->KP * pid->cur_error;
//        pid->output = -pid->output;
//        /*设定输出上限*/
//        if(pid->output > pid->outputMax) pid->output = pid->outputMax;
//        if(pid->output < -pid->outputMax) pid->output = -pid->outputMax;
//    }

}

void PID_Calc(PID_t *pid)
{
	pid->cur_error = pid->ref - pid->fdb;
	pid->output += pid->KP * (pid->cur_error - pid->error[1]) + pid->KI * pid->cur_error + pid->KD * (pid->cur_error - 2 * pid->error[1] + pid->error[0]);
	pid->error[0] = pid->error[1];
	pid->error[1] = pid->ref - pid->fdb;
	/*设定输出上限*/
	if(pid->output > pid->outputMax) pid->output = pid->outputMax;
	if(pid->output < -pid->outputMax) pid->output = -pid->outputMax;

}

void CanTransmit_GM6020_1234(CAN_HandleTypeDef *hcanx, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq){
	CAN_TxHeaderTypeDef TxMessage;
		
	TxMessage.DLC=0x08;
	TxMessage.StdId=0x1FF;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.RTR=CAN_RTR_DATA;

	uint8_t TxData[8];
	TxData[0] = (uint8_t)(cm1_iq >> 8);
	TxData[1] = (uint8_t)cm1_iq;
	TxData[2] = (uint8_t)(cm2_iq >> 8);
	TxData[3] = (uint8_t)cm2_iq;
	TxData[4] = (uint8_t)(cm3_iq >> 8);
	TxData[5] = (uint8_t)cm3_iq;
	TxData[6] = (uint8_t)(cm4_iq >> 8);
	TxData[7] = (uint8_t)cm4_iq;

	
	while(HAL_CAN_GetTxMailboxesFreeLevel(hcanx) == 0) ;
	if(HAL_CAN_AddTxMessage(hcanx,&TxMessage,TxData,&TxMailbox)!=HAL_OK)
	{
		 Error_Handler();       //如果CAN信息发送失败则进入死循环
	}
}

void CanTransmit_GM6020_567(CAN_HandleTypeDef *hcanx, int16_t cm5_iq, int16_t cm6_iq, int16_t cm7_iq) {
    CAN_TxHeaderTypeDef TxMessage;

    TxMessage.DLC = 0x08;
    TxMessage.StdId = 0x2FF;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;

    uint8_t TxData[8];
    TxData[0] = (uint8_t) (cm5_iq >> 8);
    TxData[1] = (uint8_t) cm5_iq;
    TxData[2] = (uint8_t) (cm6_iq >> 8);
    TxData[3] = (uint8_t) cm6_iq;
    TxData[4] = (uint8_t) (cm7_iq >> 8);
    TxData[5] = (uint8_t) cm7_iq;
    while (HAL_CAN_GetTxMailboxesFreeLevel(hcanx) == 0);
    if (HAL_CAN_AddTxMessage(hcanx, &TxMessage, TxData, &TxMailbox) != HAL_OK) {
        Error_Handler();       //如果CAN信息发送失败则进入死循环
    }
}

void get_dji_offset(GM6020_t *motor, uint8_t* fdbData){
	motor->FdbData.RotorAngle_0_360 = (fdbData[0]<<8|fdbData[1])*360.0f/motor->encoder_resolution;
	motor->Calculate.RotorAngle_0_360_Log[1] = motor->FdbData.RotorAngle_0_360; 
	motor->Calculate.RotorAngle_0_360_Log[NOW] = motor->Calculate.RotorAngle_0_360_Log[LAST];

	motor->Calculate.RotorAngle_0_360_OffSet = motor->FdbData.RotorAngle_0_360;
}


HAL_StatusTypeDef GM6020_CanMsgDecode(uint32_t stdid,uint8_t *fdbData) 
{
    int id =stdid-0x205;
    if( id>=0 && id<7 ){
        // GM6020_Update(id, fdbData);
		
		if(h6020s[id].FdbData.msg_cnt<50)
		{
			get_dji_offset(&h6020s[id], fdbData);
			h6020s[id].FdbData.msg_cnt++;
		}
		else
		{
			GM6020_Update(id, fdbData);
		}
        return HAL_OK;
    }

    return HAL_ERROR;
}

void GM6020_Update(int id, uint8_t *fdbData) {
	//static int last_tick[7];
	//static int ave_cnt[7];
	/*  反馈信息计算  */
	h6020s[id].FdbData.RotorAngle_0_360 = (fdbData[0] << 8 | fdbData[1]) * 360.0f
			/ h6020s[id].encoder_resolution; /* unit:degree*/
    h6020s[id].FdbData.rpm = (int16_t)(fdbData[2] << 8 | fdbData[3]);
    h6020s[id].FdbData.current = (fdbData[4] << 8 | fdbData[5]);

	/*  更新反馈速度/位置  */
	h6020s[id].Calculate.RotorAngle_0_360_Log[LAST]  =   h6020s[id].Calculate.RotorAngle_0_360_Log[NOW];
	h6020s[id].Calculate.RotorAngle_0_360_Log[NOW]   =   h6020s[id].FdbData.RotorAngle_0_360;
	
	/* 电机圈数更新        */
	if(h6020s[id].Calculate.RotorAngle_0_360_Log[NOW] -  h6020s[id].Calculate.RotorAngle_0_360_Log[LAST] > (180.0))
		h6020s[id].Calculate.RotorRound--;
	else if(h6020s[id].Calculate.RotorAngle_0_360_Log[NOW] - h6020s[id].Calculate.RotorAngle_0_360_Log[LAST] < -(180.0))
		h6020s[id].Calculate.RotorRound++;

	/* 电机输出轴角度      */
	h6020s[id].AxisData.AxisAngle_inDegree  =  h6020s[id].Calculate.RotorRound * 360.0f ;
	h6020s[id].AxisData.AxisAngle_inDegree  += h6020s[id].Calculate.RotorAngle_0_360_Log[0] - h6020s[id].Calculate.RotorAngle_0_360_OffSet;
	h6020s[id].AxisData.AxisAngle_inDegree  /= h6020s[id].reductionRate; 

	h6020s[id].AxisData.AxisVelocity        =  h6020s[id].FdbData.rpm / h6020s[id].reductionRate;
	h6020s[id].Calculate.RotorAngle_all		  =  h6020s[id].Calculate.RotorRound * 360 + h6020s[id].Calculate.RotorAngle_0_360_Log[0] - h6020s[id].Calculate.RotorAngle_0_360_OffSet;


}


void GM6020_ctrl(void){

		for(int i = 0; i <2;i++) {
//            h6020s[i].posPID.fdb = h6020s[i].AxisData.AxisAngle_inDegree;
//            h6020s[i].posPID.ref = 10.0;
            P_Calc(&h6020s[i].posPID);
            h6020s[i].speedPID.ref = h6020s[i].posPID.output;

            //speed
            // h6020s[1].speedPID.fdb = h6020s[1].FdbData.rpm;
            // h6020s[1].speedPID.ref = 60.0;



            PID_Calc(&h6020s[i].speedPID);
        }

        CanTransmit_GM6020_1234(&hcan1,
                             h6020s[0].speedPID.output,
                             h6020s[1].speedPID.output,
                             0,
                             0);

}


