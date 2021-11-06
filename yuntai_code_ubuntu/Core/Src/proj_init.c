#include "proj_init.h"


void proj_init(){

CANFilterInit(&hcan1);
GM6020_Init();

HAL_TIM_Base_Start_IT(&htim2);

HAL_UART_Receive_DMA(&huart3,joy_raw_data,JOY_BAG_LENGTH*2);    //joy:  uart3
HAL_UART_Receive_DMA(&huart6, imu_raw_data, IMU_BAG_LENGTH*2 );    //imu: uart6
HAL_UART_Receive_DMA(&huart8, yolo_raw_data, YOLO_BAG_LENGTH*2 );   //yolo: uart8

}
