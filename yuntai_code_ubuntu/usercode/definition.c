#include "definition.h"

uint8_t flag_counter = 0;

Joy_Bag_t joy_revbag;
YOLO_Bag_t yolo_revbag;
IMU_Bag_t imu_revbag;

uint8_t joy_raw_data[2*JOY_BAG_LENGTH];
uint8_t yolo_raw_data[2*YOLO_BAG_LENGTH];
uint8_t imu_raw_data[2*IMU_BAG_LENGTH];