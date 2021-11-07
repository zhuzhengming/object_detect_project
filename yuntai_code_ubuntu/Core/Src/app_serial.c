//
// Created by zhuzhengming on 2021/10/28.
//
#include "app_serial.h"


/*
currectCrc: previous crc value, set 0 if it's first section
src: source stream data
lengthInBytes: length
*/
//static void crc16_update(uint16_t *currectCrc, const uint8_t *src, uint32_t lengthInBytes)
//{
//    uint32_t crc = *currectCrc;
//    uint32_t j;
//    for (j=0; j < lengthInBytes; ++j)
//    {
//        uint32_t i;
//        uint32_t byte = src[j];
//        crc ^= byte << 8;
//        for (i = 0; i < 8; ++i)
//        {
//            uint32_t temp = crc << 1;if (crc & 0x8000)
//            {
//                temp ^= 0x1021;
//            }
//            crc = temp;
//        }
//    }
//    *currectCrc = crc;
//}

//joystick_decode

//unsigned char getCrc8(uint8_t *ptr, uint8_t len)
//{
//    uint8_t crc;
//    uint8_t i;
//    crc = 0;
//    while(len--)
//    {
//        crc ^= *ptr++;
//        for(i = 0; i < 8; i++)
//        {
//            if(crc&0x01)
//                crc=(crc>>1)^0x8C;
//            else
//                crc >>= 1;
//        }
//    }
//    return crc;
//}

void Joy_Decode(void)
{
    for (int i = 0; i < JOY_BAG_LENGTH; i++) {
        Joy_Bag_t tempBag = *(Joy_Bag_t*) (void*) (&(joy_raw_data[i]));

        if (tempBag.head[0] == JOY_HEADER_HIGH &&tempBag.head[1] == JOY_HEADER_LOW &&tempBag.state==OK)
        {
                joy_revbag = *(Joy_Bag_t*)(void*)(tempBag.raw);
        }
    }
}

void IMU_Decode(void){

    for (int i = 0; i < IMU_BAG_LENGTH; i++) {
        IMU_Bag_t tempBag = *(IMU_Bag_t*) (void*) (&(imu_raw_data[i]));

        if (tempBag.head == IMU_HEADER &&tempBag.type == IMU_TYPE )
        {
                imu_revbag = *(IMU_Bag_t*)(void*)(tempBag.raw);
        }
    }

}

void YOLO_Decode(void){

    for (int i = 0; i < YOLO_BAG_LENGTH; i++)
    {
        YOLO_Bag_t tempBag = *(YOLO_Bag_t*)(void*)(&(yolo_raw_data[i]));
        if(tempBag.head[0] == YOLO_HEADER_HIGH && tempBag.head[1] == YOLO_HEADER_LOW && tempBag.length == YOLO_BAG_LENGTH)
        {
            yolo_revbag = *(YOLO_Bag_t*)(void*)(tempBag.raw);
            flag_counter ++;
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==huart3.Instance)
    {
        Joy_Decode();
    }
    if(huart->Instance == huart6.Instance)
    {
        IMU_Decode();
    }

    if(huart->Instance == huart8.Instance)
    {
        YOLO_Decode();
    }
}
