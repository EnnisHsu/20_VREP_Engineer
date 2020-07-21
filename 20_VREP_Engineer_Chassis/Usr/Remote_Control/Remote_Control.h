/**
  ******************************************************************************
 * Copyright (c) 2020 - ~, RobotLab Development Team
  * @file   ：Remote_Control.h
  * @brief  ：Header of Remote_Control.cpp
  * @version：0.0.1
  * @changes：
 *  Date           Author   Version    Notes
 *  2019-11-30     Forsea    0.0.1     first version
  ******************************************************************************
  */

#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

 /* Includes ------------------------------------------------------------------*/
#include "Service_Engineer.h"
#include "Filter.h"
#include "conio.h"
 #ifdef __cplusplus
  extern "C" {
 #endif

 /* Private macros ------------------------------------------------------------*/
void Task_RemoteControl(void *arg);
 /* Private type --------------------------------------------------------------*/
//extern void Rescue_out();
//extern void Rescue_back();
 /* Exported macros -----------------------------------------------------------*/
extern TaskHandle_t RemoteControl_Handle;  	 
 /* Exported types ------------------------------------------------------------*/
static inline void ControllerMode();
static inline void KeyBoardMode();	  
 /* Exported function declarations --------------------------------------------*/
void Task_RemoteControl_Init();
void KeyBoardMode_Mouse_Left();
void KeyBoardMode_Mouse_Right();	 
void KeyBoardMode_Shift_Ctrl();	  
void KeyBoardMode_Shift();
void KeyBoardMode_Ctrl();
void KeyBoardMode_Normal();
#ifdef __cplusplus
}
#endif
#endif
