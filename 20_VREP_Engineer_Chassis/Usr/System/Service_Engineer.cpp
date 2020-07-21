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
#include <thread>
#include <mutex>

using namespace std;
 /* Private define ------------------------------------------------------------*/
 CEngineer_typedef Engineer_Master;
 extern CSerialPort SerialPort;
 extern VisionAlign_Classdef NUC_Obj;
 ctrl_Typedef ctrl_mess;
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
	ctrl_mess.Flag = 5;
	ctrl_mess.End = 5;
	static _Body_Operation_t* Operation_Flags;	//从各个模块获取状态
	float angle[3];
/* Pre-Load for task */
	static int a = 0;
	if (a == 0)
	{
		Engineer_Master.Init();
		myPIDTimer::getMicroTick_regist(GetMicroSecond);
		a = 1;
	}
	
	/* Infinite loop ----------------------------------------------------------*/
//	TickType_t xLastWakeTime_CenterControl;
//	xLastWakeTime_CenterControl = xTaskGetTickCount();
	//GetMicroSecond
	//for (;;)
	//{
		/* ------------------------------ Sensor Fusion ------------------------------ */
			Operation_Flags = Engineer_Master.IO_Port(BODY_OPERATION_OUT);
			/*Debug Code Below*/
			/*Debug Code Beyond*/
			Operation_Flags->detected_flag = NUC_Obj.Is_Detected();
//			cout << (int)Engineer_Master.Operations.detected_flag << "检测标志位，1则检测到" << endl;
			Engineer_Master.IO_Port(POSITION_IN, NUC_Obj.Vision_DataPack.Vision_X, NUC_Obj.Vision_DataPack.Vision_Y, NUC_Obj.Vision_DataPack.Vision_Yaw);
			/* -------------------------------------- Central Control -------------------------------- */
			Engineer_Master.center_Control();
			/* --------------------------------------- Action Excute --------------------------------- */
			//Motor Output
			if (Joint[0] != NULL)
			{
				for (int index = 0; index < 6; ++index)
				{
					Joint[index][0]->obj_Target.angVelocity_f   = (int)(Engineer_Master.IO_Port((_Unit_Type)index, WHEEL_OUT));
				}
			}
/*			if (Angle[0] != NULL)
			{
				for (int index = 0; index < 3; ++index)
				{
					angle[index] = ctrl_mess.angle[index] = Angle[index]->data;
				}
			}
			if (Body != NULL)
			{
				for (int index = 0; index < 3; ++index)
				{
					ctrl_mess.speed[index] = 
				}
			}*/
			simxGetObjectVelocity(CoppeliaSim->clientID , Body->obj_handle , ctrl_mess.speed , ctrl_mess.angle , simx_opmode_streaming);
			SerialPort.BuffSend((unsigned char*)&ctrl_mess , sizeof(ctrl_Typedef));
	//		cout << ctrl_mess.angle[0] << "  " << ctrl_mess.angle[1] << "  " << ctrl_mess.angle[2] << endl;
//			cout << angle[0] << "__" << angle[1] << "__" << angle[2] << endl;
				cout << Joint[0][0]->obj_Target.angVelocity_f << 'A' << Joint[1][0]->obj_Target.angVelocity_f << 'A' << Joint[2][0]->obj_Target.angVelocity_f << 'A' << Joint[3][0]->obj_Target.angVelocity_f << 'A' << Joint[4][0]->obj_Target.angVelocity_f << 'A' << Joint[5][0]->obj_Target.angVelocity_f << endl;
	//		CoppeliaSim.ComWithServer();

		/* Pass control to the next task ------------------------------------------*/
//		Sleep(5);
	//}
}
 /* function prototypes -------------------------------------------------------*/

