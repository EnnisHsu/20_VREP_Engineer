/**
  ******************************************************************************
 * Copyright (c) 2020 - ~, RobotLab Development Team
  * @file   ：Service_Engineer.h
  * @brief  ：Header of Service_Engineer.cpp
  * @version：0.0.1
  * @changes：
 *  Date           Author   Version    Notes
 *  2019-11-27     Forsea    0.0.1     first version
  ******************************************************************************
  */

#ifndef SERVICE_ENGINEER_H
#define SERVICE_ENGINEER_H

 /* Includes ------------------------------------------------------------------*/
//#include "System_DataPool.h"
#include "Engineer.h"
#include "Motor_C620.h"
#include "CoppeliaSim_service.h"
#include "win32_support.h"
 #ifdef __cplusplus
  extern "C" {
 #endif

 /* Private macros ------------------------------------------------------------*/
 /* Private type --------------------------------------------------------------*/
    
//    extern Motor_Ctrl Engineer_Motors;
 /* Exported macros -----------------------------------------------------------*/
extern TaskHandle_t EngineerControl_Handle;
void   Task_EngineerControl(void *arg);	 

void Service_Engineer_Init(void);
 /* Exported types ------------------------------------------------------------*/
extern float current[6];
extern CSerialPort visual_Port;

 /* Exported function declarations --------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif
