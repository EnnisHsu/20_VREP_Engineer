/**
******************************************************************************
* Copyright (c) 2020 - ~, SCUT-RobotLab Development Team
* @file    mycontroller.cpp
* @author  Forsea
* @brief   
* @date    2019-12-08
* @version 1.0
* @par Change Log£º
* <table>
* <tr><th>Date        <th>Version  <th>Author    		<th>Description
* <tr><td>2019-12-08  <td> 1.0     <td>Forsea             <td>Creator
* <tr><td>2020-03-29  <td> 2.0     <td>EnnisKoh           <td>Add upper control code
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
#include "mycontroller.h"
#include "iostream"
using namespace std;

 /* Private define ------------------------------------------------------------*/
myPID Wheel_LF(10,0.1,0),
      Wheel_RF(10,0.1,0),
      Wheel_RM(10,0.1,0),
      Wheel_RB(10,0.1,0),
      Wheel_LB(10,0.1,0),
      Wheel_LM(10,0.1,0),
	  Pose_X(1,1,0),
	  Pose_Y(1,1,0),
	  Pose_Yaw(1,1,0);
	  
	  
	  
	  
myPID* chassis_Speed[WHEEL_NUM] = {&Wheel_LF, &Wheel_RF, &Wheel_RB, &Wheel_LB, &Wheel_LM, &Wheel_RM};	 ;
 /* Private variables ---------------------------------------------------------*/

 /* Private type --------------------------------------------------------------*/

 /* Private function declarations ---------------------------------------------*/
float myConstrain(float input,float min,float max){
  if (input <= min)
    return min;
  else if(input >= max)
    return max;
  else return input;
}

void mycontroller_init()
{
	for(int index = 0; index < WHEEL_NUM; index++)
	{
		chassis_Speed[index]->I_Term_Max = 60;
		chassis_Speed[index]->DeadZone = 10;
		chassis_Speed[index]->I_SeparThresh = 30;
		chassis_Speed[index]->Out_Max = 96;
		
	}
	
	Pose_X.DeadZone = 10;
	Pose_X.I_Term_Max = 50;
	Pose_X.I_SeparThresh = 1000;
	Pose_X.Out_Max = 100;
	
	Pose_Y.DeadZone = 10;
	Pose_Y.I_Term_Max = 50;
	Pose_Y.I_SeparThresh = 1000;
	Pose_Y.Out_Max = 100;
	
	Pose_Yaw.DeadZone = 10;
	Pose_Yaw.I_Term_Max = 50;
	Pose_Yaw.I_SeparThresh = 1000;
	Pose_Yaw.Out_Max = 100;
	
}

extern CEngineer_typedef Engineer_Master;
int32_t* Controller_ChassisSpeedLoop(const int16_t* currentSpeed, const int16_t* targetSpeed)
{
  static int32_t Output_Buf[WHEEL_NUM];
  uint8_t i = 0;

  for(i = 0 ; i < WHEEL_NUM ; ++i)
  {
    /* Speed loop */
    chassis_Speed[i]->Current = *(currentSpeed + i);
    chassis_Speed[i]->Target  = *(targetSpeed  + i);
    chassis_Speed[i]->Adjust();
    Output_Buf[i] = chassis_Speed[i]->Out;
    if(Engineer_Master.get_CurrentMode() == UP_STAIRS && Engineer_Master.upstairs_Mode == Manual)
    {
      if(i == 2 || i ==3)
        Output_Buf[i] = *(targetSpeed  + i);
    }
    /* Differ speed loop */
    
  }
  
  return Output_Buf;
}

_chassis_Velocity* Controller_ChassisPositionLoop(const _chassis_GlobalPos* CurrentPose, const _chassis_GlobalPos* TargetPose)
{
	static  _chassis_Velocity target_Velocity;
	
	Pose_X.Current = CurrentPose->x;
	Pose_Y.Current = CurrentPose->y;
	Pose_Yaw.Current = CurrentPose->yaw;
	Pose_X.Target = myConstrain(TargetPose->x,-1000,1000);
	Pose_Y.Target = myConstrain(TargetPose->y,-1500,1500);
	Pose_Yaw.Target = TargetPose->yaw;
	
	Pose_X.Adjust();
	Pose_Y.Adjust();
	Pose_Yaw.Adjust();
	
	target_Velocity.x_speed = myConstrain(Pose_X.Out,-100,100);
	target_Velocity.y_speed = myConstrain(Pose_Y.Out,-100,100);
	target_Velocity.z_speed = myConstrain(Pose_Yaw.Out,-1000,1000);
//	cout  << CurrentPose->x << 'A' << TargetPose->x << 'A' << Pose_Yaw.Out << endl;
	return &target_Velocity;
}

 /* function prototypes -------------------------------------------------------*/
