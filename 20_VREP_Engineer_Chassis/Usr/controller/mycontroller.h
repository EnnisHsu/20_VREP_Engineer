/**
  ******************************************************************************
 * Copyright (c) 2020 - ~, RobotLab Development Team
  * @file   £ºmycontroller.h
  * @brief  £ºHeader of mycontroller.cpp
  * @version£º0.0.1
  * @changes£º
 *  Date           Author   Version    Notes
 *  2019-12-08     Forsea    0.0.1     first version
  ******************************************************************************
  */

#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

 /* Includes ------------------------------------------------------------------*/
#include "PID.h"
#include "math.h"
#include "Engineer.h"
#include "mypid.h"
 #ifdef __cplusplus
  extern "C" {
 #endif

 /* Private macros ------------------------------------------------------------*/

 /* Private type --------------------------------------------------------------*/

 /* Exported macros -----------------------------------------------------------*/
int32_t* Controller_ChassisSpeedLoop(const int16_t* currentSpeed, const int16_t* targetSpeed);
_chassis_Velocity* Controller_ChassisPositionLoop(const _chassis_GlobalPos* CurrentPose, const _chassis_GlobalPos* TargetPose);
void mycontroller_init();	  
 /* Exported types ------------------------------------------------------------*/

 /* Exported function declarations --------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif
