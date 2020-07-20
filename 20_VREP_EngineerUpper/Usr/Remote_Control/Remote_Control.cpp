/**
******************************************************************************
* Copyright (c) 2020 - ~, SCUT-RobotLab Development Team
* @file    Remote_Control.cpp
* @author  Forsea
* @brief   
* @date    2019-11-30
* @version 1.0
* @par Change Log：
* <table>
* <tr><th>Date        <th>Version  <th>Author    		<th>Description
* <tr><td>2019-11-30  <td> 1.0     <td>Forsea             <td>Creator
* <tr><td>2020-03-29  <td> 2.0     <td>EnnisKoh           <td>Add Upper Control Code
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
#include "Remote_Control.h"
#include "UpperMonitor.h"
#include "iostream"
#include "Vision_EngineerAlign.h"
#include "Service_UpperCtrl.h"

using namespace std;
 /* Private define ------------------------------------------------------------*/
TaskHandle_t RemoteControl_Handle; 

VisionAlign_Classdef NUC_Obj;

float  sensitivity_z = 2; 
char key;
extern  CEngineer_typedef Engineer_Master;
 /* Private variables ---------------------------------------------------------*/
void Task_RemoteControl_Init()
{
	xTaskCreate(Task_RemoteControl, "App.Remote Ctrl", Large_Stack_Size, NULL, PriorityHigh, &RemoteControl_Handle);
}
 /* Private type --------------------------------------------------------------*/

 /* Private function declarations ---------------------------------------------*/
void Task_RemoteControl(void *arg)
{
//	static COB_TypeDef* RelayCmdBuf;
	TickType_t xLastWakeTime_Remote;
	xLastWakeTime_Remote = xTaskGetTickCount();
	
	Engineer_Master.switch_Mode(NORMAL);
	for(;;)
	{
			  KeyBoardMode();
		vTaskDelayUntil(&xLastWakeTime_Remote, 50);
	}
}

static inline void ControllerMode()
{

}

static inline void KeyBoardMode()
{
	static int flag;
  static float MouseX = 0;
	  if (_kbhit())
	  {
		  key = getch();
	  }
//		  key = bioskey(1);
//   cout << key;
//  static float MouseX_SliderFliter[4] = {0};
//  static LowPassFilter MouseX_LowPassFliter(0.7);
//  static uint8_t Filter_index = 0;
//  
//  /*鼠标移动信息*/
//  MouseX = MouseX_LowPassFliter.f(MouseX);
//  MouseX = Slider_Filter(MouseX_SliderFliter,&Filter_index,4,DR16.Get_MouseX_Norm());

  /*  
    鼠标点击信息 
  */
//  if(DR16.IsKeyPress(_Mouse_L))
 //   KeyBoardMode_Mouse_Left();
 // if(DR16.IsKeyPress(_Mouse_R))
 //   KeyBoardMode_Mouse_Right();
//  else{
//    if(Engineer_Master.Get_CurrentMode() == AUTO_ROTATE && Engineer_Master.Operations.auto_rotate_flag == CAT_WALK)
//      Engineer_Master.Switch_ActionMode(NORMAL);
//  }
  
  /* 
    按键信息 ：运动按键
  */
  static float temp_X_Co = 0, temp_Y_Co = 0;
  if(key == 'w')
	  temp_Y_Co++;
  if(key == 'a') temp_X_Co -= 0.8f;
  if(key == 's') temp_Y_Co --;
  if(key == 'd') temp_X_Co += 0.8f;
//  //设置手动输入目标
  Engineer_Master.set_Target(temp_X_Co, temp_Y_Co, sensitivity_z * MouseX);// sensitivity_z*MouseX
  temp_Y_Co = temp_X_Co = 0;
  
  /* 
    按键信息 ：功能按键
  */
  //Shift + Crtl
//  if(DR16.IsKeyPress(_SHIFT) && DR16.IsKeyPress(_CTRL))
 //    KeyBoardMode_Shift_Ctrl();
  //Shift
//if(DR16.IsKeyPress(_SHIFT))
 //     KeyBoardMode_Shift();
  //Ctrl
 // else if(DR16.IsKeyPress(_CTRL))
 //     KeyBoardMode_Ctrl();
//  else
      KeyBoardMode_Normal();
	  key = '0';
}

void KeyBoardMode_Mouse_Left()
{
	if(Engineer_Master.get_CurrentMode() == GET_BULLET)
	{
//		Engineer_Relay.write_Cmd(Mouse_Left);
	}
}
void KeyBoardMode_Mouse_Right()
{
	if(Engineer_Master.get_CurrentMode() == GET_BULLET)
	{
//		Engineer_Relay.write_Cmd(Mouse_Right);
	}
	else
	{
		Engineer_Master.switch_Mode(AUTO_ROTATE);  //WSAD取消自旋，切换正常模式
		Engineer_Master.Operations.auto_rotate_flag = CAT_WALK;
	}
}

void KeyBoardMode_Shift_Ctrl()
{
/*	if(Engineer_Master.get_CurrentMode() == GET_BULLET)
	{
		if(DR16.IsKeyPress(_G)){Engineer_Relay.write_Cmd(Ctrl_Shift_G);}
		if(DR16.IsKeyPress(_F))
		{
			if(Engineer_Master.Operations.get_bullet_flag == APPROACH)
			  NUC_Obj.SetTargetMode(ISLAND_TWO);
			//第二次对准(第一次对准中断后进入手动对准，手动模式之后再开就是对右边)
			else if(Engineer_Master.Operations.get_bullet_flag == MANUAL)
			{
			  NUC_Obj.SetTargetMode(ISLAND_TWO);
			  Engineer_Master.Operations.get_bullet_flag = ALIGNING_AGAIN;
			}			
			Engineer_Relay.write_Cmd(Ctrl_Shift_F);
		}
	}
	if(DR16.IsKeyPress(_V)){Engineer_Relay.write_Cmd(Ctrl_Shift_V);}
	if(DR16.IsKeyPress(_B))
	{
		Engineer_Master.switch_Mode(NORMAL);         //复位
		NUC_Obj.SetTargetMode(RERUN);
		Engineer_Master.camera_Flag = FORWARD;
		Engineer_Relay.write_Cmd(B);
	}	
	*/
}

void KeyBoardMode_Shift()
{
/*	Engineer_Master.gear_flag = SPEED_UP;
	if(Engineer_Master.get_CurrentMode() == GET_BULLET)
	{
		if(DR16.IsKeyPress(_F))
		{  
//			if(Engineer_Master.island_flag == 0)
//			  NUC_Obj.SetTargetMode(GROUND_MID);
//			else
//			  NUC_Obj.SetTargetMode(ISLAND_MID);
			Engineer_Relay.write_Cmd(Shift_F);
		}
		if(DR16.IsKeyPress(_Z)){Engineer_Relay.write_Cmd(Shift_Z);}
		if(DR16.IsKeyPress(_X)){Engineer_Relay.write_Cmd(Shift_X);}
		if(DR16.IsKeyPress(_C)){Engineer_Relay.write_Cmd(Shift_C);}
	}
	if(DR16.IsKeyPress(_G))
	{
		Engineer_Master.switch_Mode(NORMAL);
		Engineer_Relay.write_Cmd(Shift_G);
	}
	if(DR16.IsKeyPress(_V))
	{
		Engineer_Master.switch_Mode(NORMAL);
		Rescue_back();
	}	
	
    if(DR16.IsKeyPress(_R))
	{
		Engineer_Master.switch_Mode(GIVE_BULLET_NORMAL);
		Engineer_Relay.write_Cmd(Shift_R);
	}
	
	if(DR16.IsKeyPress(_B))
	{
		Engineer_Master.switch_Mode(NORMAL);         //复位
		NUC_Obj.SetTargetMode(RERUN);
	    Engineer_Master.camera_Flag = FORWARD;
		Engineer_Relay.write_Cmd(B);
	}*/
}

void KeyBoardMode_Ctrl()
{
/*	static bool reboot_flag = 0;
	Engineer_Master.gear_flag = SLOW_DOWN;
	if(Engineer_Master.get_CurrentMode() == GET_BULLET)
	{
		if(DR16.IsKeyPress(_G)){Engineer_Relay.write_Cmd(Ctrl_G);}
		if(DR16.IsKeyPress(_F))
		{
			if(Engineer_Master.Operations.get_bullet_flag == APPROACH)
			  NUC_Obj.SetTargetMode(ISLAND_THREE);
			//第二次对准(第一次对准中断后进入手动对准，手动模式之后再开就是对右边)
			else if(Engineer_Master.Operations.get_bullet_flag == MANUAL)
			{
			  NUC_Obj.SetTargetMode(ISLAND_THREE);
			  Engineer_Master.Operations.get_bullet_flag = ALIGNING_AGAIN;
			}
			Engineer_Relay.write_Cmd(Ctrl_F);
		}
		if(DR16.IsKeyPress(_V)){Engineer_Relay.write_Cmd(Ctrl_V);}	
	}
	
    if(DR16.IsKeyPress(_Z))
	{
		Engineer_Master.switch_Mode(NORMAL);
	}
    if(DR16.IsKeyPress(_X))
	{
		Engineer_Master.switch_Mode(UP_STAIRS);
	}
    if(DR16.IsKeyPress(_C))
	{
		Engineer_Master.switch_Mode(GET_BULLET);
	}

	
	if(DR16.IsKeyPress(_B))
	{
		Engineer_Master.switch_Mode(NORMAL);         //复位
		NUC_Obj.SetTargetMode(RERUN);
	    Engineer_Master.camera_Flag = FORWARD;
		Engineer_Relay.write_Cmd(B);
	}*/
}

void KeyBoardMode_Normal()
{
	Engineer_Master.gear_flag = NO_CHANGE;
	if(Engineer_Master.get_CurrentMode() == GET_BULLET)
	{
		if(key == 'f')
		{
			NUC_Obj.SetTargetMode(GROUND_LEFT);
			if (Engineer_Master.Operations.get_bullet_flag == APPROACH)
			{
				NUC_Obj.SetTargetMode(GROUND_LEFT);
				SerialPort.BuffSend((unsigned char*)NUC_Obj.Get_TxBuff(), sizeof(Vision_TxData_Typedef));//NUC_Obj.Get_TxBuff()
			}
			
			//第二次对准(第一次对准中断后进入手动对准，手动模式之后再开就是对右边)
			else if(Engineer_Master.Operations.get_bullet_flag == MANUAL)
			{
			  NUC_Obj.SetTargetMode(GROUND_LEFT);
			  Engineer_Master.Operations.get_bullet_flag = ALIGNING_AGAIN;
			}
//			Engineer_Relay.write_Cmd(F);
		}
		if (GetKeyState(VK_LCONTROL) < 0)
		{
			if (_kbhit())
			{
				key = getch();
			}
			switch (key)
			{
			case 'z'&0x1f:
				Slide_Left();
				StretchOut();
				break;
			case 'x' & 0x1f:
				Slide_Med();
				StretchOut();
				break;
			case 'c' & 0x1f:
				Slide_Right();
				StretchOut();
				break;
			case 'r' & 0x1f:
				Lift_Top();
				//std::cout << "Key_" << (char)ch << " pressed\n";
				break;
			case 'f' & 0x1f:
				Lift_Med();
				//std::cout << "Key_" << (char)ch << " pressed\n";
				break;
			case 'v' & 0x1f:
				Lift_Btm();
				//std::cout << "Key_" << (char)ch << " pressed\n";
				break;
			default:
				break;
			}
		}
		else
		{
			if (_kbhit())
			{
				key = getch();
			}
			switch (key)
			{
			case 'z':
				Slide_Left();
				StretchBack();
				//std::cout << "Key_" << (char)ch << " pressed\n";
				//SysLog.Record(_INFO_, "Interface", "Key_Z pressed");
				break;
			case 'x':
				Slide_Med();
				StretchBack();
				//std::cout << "Key_" << (char)ch << " pressed\n";
				break;
			case 'c':
				Slide_Right();
				StretchBack();
				//std::cout << "Key_" << (char)ch << " pressed\n";
				break;
			case 't':
				Rotate();
				//std::cout << "Key_" << (char)ch << " pressed\n";
				break;
			case 'y':
				RotateAllBack();
				//std::cout << "Key_" << (char)ch << " pressed\n";
				break;
			case 'u':
				StretchOut();
				//std::cout << "Key_" << (char)ch << " pressed\n";
				break;
			case 'i':
				StretchBack();
				//std::cout << "Key_" << (char)ch << " pressed\n";
				break;
			case 'o':
				Pinch();
				break;
			case 'p':
				Loosen();
				break;
			case 'b':
				Bullet_Status = Auto_Reset;
				//std::cout << "Key_" << (char)ch << " pressed\n";
				break;
			case 'g':
				Bullet_Task_Status = Task_Auto_1Box;
				Bullet_Status = Lifting;
				std::cout << "Task Auto 1Box Start\n";
				break;
			case 'h':
				Bullet_Task_Status = Task_Auto_2Boxes_of_One;
				Bullet_Status = Lifting;
				std::cout << "Task Auto 2Boxes Start\n";
				break;
			case 'j':
				Bullet_Task_Status = Task_Auto_4Boxes_of_One;
				Bullet_Status = Lifting;
				std::cout << "Task Auto 4Boxex Start\n";
			case 'n':
				//SetDummyLink(1);
				std::cout << "Link FM\n";
				break;
			case 'm':
				SetDummyLink(0);
				std::cout << "DisLink All\n";
				break;
			default:
				break;
			}
		}
/*		if(DR16.IsKeyPress(_Z)){Engineer_Relay.write_Cmd(Z);}
		if(DR16.IsKeyPress(_X)){Engineer_Relay.write_Cmd(X);}
		if(DR16.IsKeyPress(_C)){Engineer_Relay.write_Cmd(C);}*/
	}
	
	if(key == 'g')
	{
		Engineer_Master.switch_Mode(GET_BULLET);
		Lift_Med();
	}
	
//	自旋操作，通过按Q、E进入自旋模式，松开就退出
	if(key == 'e')
	{
		Engineer_Master.switch_Mode(AUTO_ROTATE);      //自动向左旋转180°，自动切换NORMAL模式
		Engineer_Master.set_AutoRotate(SINGLE);	
	}
	if(key == 'q')
	{
		Engineer_Master.switch_Mode(AUTO_ROTATE);      //长按自动向右高速自旋
		Engineer_Master.set_AutoRotate(CONTINUOUS);
	}
	else
    { //不按就退出自旋
		if(Engineer_Master.get_CurrentMode() == AUTO_ROTATE && Engineer_Master.Operations.auto_rotate_flag == CONTINUOUS)
		Engineer_Master.switch_Mode(NORMAL);
	}	

	if(key == 'v')
	{
		Engineer_Master.switch_Mode(RESCUE);
//	Rescue_out();
	}

	if(key == 'r')
	{
//		Engineer_Relay.write_Cmd(R);
	}
	
	if(key == 'b')
	{
		Bullet_Status = Auto_Reset;			// 上层自动复位
		Engineer_Master.switch_Mode(NORMAL);         //复位
//		NUC_Obj.SetTargetMode(RERUN);
	    Engineer_Master.camera_Flag = FORWARD;
// 		Engineer_Relay.write_Cmd(B);
	}	
}
 /* function prototypes -------------------------------------------------------*/

