/**
  ******************************************************************************
  * @file   System_DataPool.c
  * @brief  All used resources are contained in this file.
  ******************************************************************************
  * @note
  *  - User can define datas including variables ,structs ,and arrays in
  *    this file, which are used in deffrient tasks or services.
**/
#include "System_DataPool.h"

/* RTOS Resources ------------------------------------------------------------*/
/* Queues */
/* Semaphores */
/* Mutexes */
/* Notifications */

/* Other Resources -----------------------------------------------------------*/
CLogger SysLog;
LogFilter_t Filter_List[2];

_simObjectHandle_Type* Body;
_simObjectHandle_Type* Joint[6][LEG_JOINT];
//_simObjectHandle_Type* Angle[3];


_simSignalHandle_Type* Angle[3];
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/



