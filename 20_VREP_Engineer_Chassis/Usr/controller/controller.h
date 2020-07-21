/**
  **********************************************************************************
  * @file   : controller.h
  * @brief  : Header for controller(Robomaster 2019 Engineer version 4.0) robot lib.
  * @author : Mentos_Seetoo
  * @date   : July,2019
  * @version: 1.0
  **********************************************************************************
**/

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "chassis.h"
#include "PID.h"
#include "Tool_PID.h"
#include "Filter.h"
#ifdef  __cplusplus
/* Private  ------------------------------------------------------------------*/
template<typename Type>
Type myConstrain(Type input, Type min, Type max)
{
  if (input <= min)
    return min;
  else if (input >= max)
    return max;
  else
    return input;
}
/* Exported ------------------------------------------------------------------*/

extern FUZZYPID_without_gyroscope Pose_XAxis, Pose_YAxis, Pose_Yaw;
void Controller_Init(void);
_chassis_Velocity* Controller_ChassisAttitudeLoop(const _chassis_GlobalPos* CurrentPose, const _chassis_GlobalPos* TargetPose);
_chassis_Velocity* Controller_ChassisPositionLoop(const _chassis_GlobalPos* CurrentPose, const _chassis_GlobalPos* TargetPose);
int32_t* Controller_ChassisSpeedLoop(const int16_t* currentSpeed, const int16_t* targetSpeed);
void Controller_SetMaxAdjustSpeed(float max_adjust_speed);
//myPID* Chassis_Speed[WHEEL_NUM] = {&Wheel_LF, &Wheel_RF, &Wheel_RB, &Wheel_LB, &Wheel_LM, &Wheel_RM};
#endif
#endif
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
