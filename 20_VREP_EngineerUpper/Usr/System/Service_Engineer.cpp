/**
******************************************************************************
* Copyright (c) 2020 - ~, SCUT-RobotLab Development Team
* @file    Service_Engineer.cpp
* @author  Forsea
* @brief   
* @date    2019-11-27
* @version 1.0
* @par Change Log：
* <table>
* <tr><th>Date        <th>Version  <th>Author    		<th>Description
* <tr><td>2019-11-27  <td> 1.0     <td>Forsea             <td>Creator
* </table>
*
==============================================================================
                    ##### How to use this driver #####
==============================================================================
  @note


  @warning


******************************************************************************
* @attention
* 

******************************************************************************
*/
 /* Includes ------------------------------------------------------------------*/
#include "Service_Engineer.h"
#include "iostream"
#include "System_DataPool.h"
#include "time.h"
#include "win32_support.h"
#include "SerialPort.h "
#include "UpperMonitor.h"
#include "Vision_EngineerAlign.h"

using namespace std;
 /* Private define ------------------------------------------------------------*/
 CEngineer_typedef Engineer_Master;
 extern CSerialPort SerialPort;
 extern VisionAlign_Classdef NUC_Obj;
 extern int COM_Callback(unsigned char* data, unsigned int num);
  float current[6];
/**
* @brief  Initialization of Engineer service
* @param  None.
* @return None.
*/
void Service_Engineer_Init(void)
{
  //Create service task
  xTaskCreate(Task_EngineerControl,"Robo.Engineer Ctrl", Small_Stack_Size, NULL,PriorityHigh, &EngineerControl_Handle);
}

/*------------------------------- Engineer Control -----------------------------*/
/*Task Define ---------------------------*/
TaskHandle_t EngineerControl_Handle;    
 /* Private variables ---------------------------------------------------------*/
    
 /* Private type --------------------------------------------------------------*/

 /* Private function declarations ---------------------------------------------*/
void   Task_EngineerControl(void *arg)
{
  /* Cache for Task */
	static Vision_DataPack_Typedef vision_data;
	static _Body_Operation_t *Operation_Flags;	//从各个模块获取状态
	static int16_t wheel_rpmOut[6];
	static char CV_rec[3];
	static CV_data current_Pos;
  /* Pre-Load for task */
	Engineer_Master.Init();
  /* Infinite loop ----------------------------------------------------------*/
	TickType_t xLastWakeTime_CenterControl;
	xLastWakeTime_CenterControl = xTaskGetTickCount();
	myPIDTimer::getMicroTick_regist(GetMicroSecond);
	for(;;)
	  {		
		/* ------------------------------ Sensor Fusion ------------------------------ */
		Operation_Flags = Engineer_Master.IO_Port(BODY_OPERATION_OUT);
		/*Debug Code Below*/
		/*Debug Code Beyond*/
//cout << current[0] << "C__" << current[1] << "C__" << current[2] << "C__" << current[3] << "C__" << current[4] << "C__" << current[5] << endl;
		Operation_Flags->detected_flag = NUC_Obj.Is_Detected();
		//cout << (int)Engineer_Master.Operations.detected_flag << "检测标志位，1则检测到" << endl;
		Engineer_Master.IO_Port(POSITION_IN, NUC_Obj.Vision_DataPack.Vision_X, NUC_Obj.Vision_DataPack.Vision_Y, NUC_Obj.Vision_DataPack.Vision_Yaw);
		//导入PID的当前值
		for(int index = 0;index < WHEEL_NUM; index++)
		  {
				Engineer_Master.IO_Port((_Unit_Type)index,RPM_IN,((int16_t)current[index]));
		  }
		/* -------------------------------------- Central Control -------------------------------- */
		Engineer_Master.center_Control();
		/* --------------------------------------- Action Excute --------------------------------- */
		//Motor Output
		if (Joint[0] != NULL)
		{
			for (int index = 0; index < 6; ++index)
			{
				Joint[index][0]->obj_Target.angVelocity_f = (int)(Engineer_Master.IO_Port((_Unit_Type)index, WHEEL_OUT));
			}
		}
//		cout << Joint[0][0]->obj_Target.angVelocity_f << 'A' << Joint[1][0]->obj_Target.angVelocity_f << 'A' << Joint[2][0]->obj_Target.angVelocity_f << 'A' << Joint[3][0]->obj_Target.angVelocity_f << 'A' << Joint[4][0]->obj_Target.angVelocity_f << 'A' << Joint[5][0]->obj_Target.angVelocity_f << endl;
		CoppeliaSim.ComWithServer();

		

	  /* Pass control to the next task ------------------------------------------*/
		vTaskDelayUntil(&xLastWakeTime_CenterControl,50);
	}
}
 /* function prototypes -------------------------------------------------------*/

