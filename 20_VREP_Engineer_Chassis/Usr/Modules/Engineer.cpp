/**
******************************************************************************
* Copyright (c) 2020 - ~, SCUT-RobotLab Development Team
* @file    Engineer.cpp
* @author  Forsea
* @brief   
* @date    2019-11-10
* @version 1.0
* @par Change Log：
* <table>
* <tr><th>Date        <th>Version  <th>Author    		<th>Description
* <tr><td>2019-11-10  <td> 1.0     <td>Forsea             <td>Creator
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
#include "Engineer.h"  
#include "iostream"
using namespace std;
 /* Private define ------------------------------------------------------------*/

 /* Private variables ---------------------------------------------------------*/
    
 /* Private type --------------------------------------------------------------*/

 /* Private function declarations ---------------------------------------------*/
 _ActionMode_t CEngineer_typedef::get_CurrentMode()
 {
	return mode;
 }
 
 /**
 *@brief	Engineer初始化
 *@note		加载控制环，初始化参数，设置默认初始模式为NORMAL
 *@param	NULL
 *@retval	NULL
 *@author
 */
 
 
 void CEngineer_typedef::Init()
{
//  SelfCheck_Status();
  /* Controller Init */
	mycontroller_init();
	Lunar_Chassis.Load_PositionController(Controller_ChassisPositionLoop);
	Lunar_Chassis.Load_SpeedController(Controller_ChassisSpeedLoop);
//  Lunar_Chassis.Load_AttitudeController(Controller_ChassisAttitudeLoop);
  
  /* Motion Paramter Init */
//  up_stairs_co1   = UP_STAIRS_MOVE_SPEED;
//  up_stairs_co2   = UP_STAIRS_UP_SPEED;
//  up_stairs_pitch = UP_STAIRS_STOP_PITCH;
//  align_Y_co      = ALIGN_Y_SPEED;
//  threshold_time  = THRESHOLD_TIME;
  
	mode = NORMAL;
}
 
 /**
* @brief  需要修改的参数一般有:
*         1.底盘模式:位置/速度模式
*         2.速度档位:高中低
*         3.指令源(自动/手动指令)
*         4.视角:深度/模组/图传
*         5.启动力矩优化/陀螺仪优化/倾角保护
* @param  None.
* @return robot state
*/
void CEngineer_typedef::center_Control()
{
  //Self check.
//  SelfCheck_Status();
  
  //Select action.
//  Lunar_Chassis.Update_CurrentAttitude(IMU_Data.roll, IMU_Data.pitch, IMU_Data.yaw);
  switch(mode)
  {
    case CHASSIS_LOCK:
      /*
         紧急停止
      */
      Lunar_Chassis.Switch_Mode(Halt);
      break;
    
    case NORMAL:
      /*
        1.速度模式
        2.默认中档
        3.手动指令
        4.图传视角
        5.全优化开
      */
      action_Normal();
      break;

    case GET_BULLET:
      /*
        1.自动/手动混合控制
        2.默认抵挡
        3.深度视角
        4.倾角保护
      */
      action_Get_Bullet();
      break;
    
    case UP_STAIRS:
      /*
        1.自动/手动混合控制，速度模式
        2.默认低挡
        3.模组视角
        4.倾角保护
      */
      action_Upstairs();
      break;
    
    case RESCUE:
      /*
        1.自动/手动混合控制，速度模式
        2.图传切模组
        3.救援后切入快速档
        4.力矩优化
      */
      action_Rescue();
      break;
    
    case AUTO_ROTATE:
      /*
        自动旋转180°/自动旋转∞°
      */
      action_Auto_Rotate();
      break;
    
    case GIVE_BULLET_NORMAL:
      action_Give_Bullet();
      break;
  }
  
  //Speed control for manual input.
  GearBox();
  
  //Chassis control & clear the command.
  Lunar_Chassis.Chassis_Control();
}
 /**
 *@brief	用于切换工程的工作模式
 *@note		1、不同模式下可以切换的模式不同
			2、切换的时候可以先设置一次速度挡位，之后某些模式可以遥控更改
			3、根据当前模式和目标模式的摄像头状态是否相同，考虑调整摄像头方向
 *@param	(_ActionMode_t) targetMode：将要切换的目标模式
 *@retval	NULL
 *@author	
 */
 
 
 void CEngineer_typedef::switch_Mode(_ActionMode_t targetMode)
 {
/* Switching by current mode */
  switch(mode)
  {
    /* 
      取弹模式
    */
    case GET_BULLET:
      switch(targetMode)
      {
        case NORMAL:
		  Manual_Target.x = 0;
          Manual_Target.y = 0;
          Manual_Target.z = 0;
          Lunar_Chassis.Set_SpeedGear(NORMAL_GEAR);
          Lunar_Chassis.Set_TorqueOptimizeFlag(ON);
          camera_Flag = FORWARD;
          mode = NORMAL;
          break;
        default:
          //除了复位以外不切换其他状态，取弹状态优先级最高，防止误触
          break;
      }
      break;
      
    /* 
       自动旋转模式
    */
    case AUTO_ROTATE:
      cat_walk_flag = 0;
      switch(targetMode)
      {
        case NORMAL:
          mode = NORMAL;
          //清除旧指令
          Manual_Target.x = 0;
          Manual_Target.y = 0;
          Manual_Target.z = 0;
          Lunar_Chassis.Set_SpeedGear(NORMAL_GEAR);
          Lunar_Chassis.Set_TorqueOptimizeFlag(ON);
        break;
        
        case GET_BULLET:
          mode = GET_BULLET;
          //清除旧指令
          Manual_Target.x = 0;
          Manual_Target.y = 0;
          Manual_Target.z = 0;
          Lunar_Chassis.Set_SpeedGear(SLOW_GEAR);
          Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
          camera_Flag       = BULLET;
          Operations.get_bullet_flag = APPROACH;
          Operations.detected_flag   = 0;
        break;
        
        case AUTO_ROTATE:
          mode = AUTO_ROTATE;
          //清除旧指令
          Manual_Target.x = 0;
          Manual_Target.y = 0;
          Manual_Target.z = 0;
          if(Operations.auto_rotate_flag == CONTINUOUS)
            Lunar_Chassis.Update_ZeroPose();
        break;
        
        default:
          mode = targetMode;		//？？
          //自动旋转状态可切换任意状态
        break;
      }
    break;
    
    /* 
      上岛模式 
    */
    case UP_STAIRS:
      switch(targetMode)
      {
        case NORMAL:
          mode = NORMAL;
          Manual_Target.x = 0;
          Manual_Target.y = 0;
          Manual_Target.z = 0;
          Lunar_Chassis.Set_SpeedGear(NORMAL_GEAR);
          Lunar_Chassis.Set_TorqueOptimizeFlag(ON);
          camera_Flag = FORWARD;
        break;
        
        case GET_BULLET:
          mode = GET_BULLET;
          Lunar_Chassis.Set_SpeedGear(SLOW_GEAR);
          Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
          camera_Flag       = BULLET; 
          Operations.get_bullet_flag = APPROACH;
          Operations.detected_flag   = 0;
          upstairs_Mode   = Manual;
		  Operations.upstairs_flag = STEP1;
        break;
        
        case UP_STAIRS:
          //手动模式下切回自动
          if(upstairs_Mode == Manual)
          {
            mode = UP_STAIRS;
            Lunar_Chassis.Set_SpeedGear(NORMAL_GEAR);
            Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
            camera_Flag     = FORWARD;
            Operations.upstairs_flag = STEP1;
          }
        break;
        default:
          //上岛状态下只能复位
          break;
      }
    break;
      
    /* 
      其他状态下默认切换 
    */
    default:
      switch(targetMode)
      {
        case CHASSIS_LOCK:
          mode = NORMAL;		//????
          break;
        case NORMAL:
          mode = NORMAL;
          Lunar_Chassis.Set_SpeedGear(NORMAL_GEAR);
          Lunar_Chassis.Set_TorqueOptimizeFlag(ON);
          Operations.upstairs_flag = STEP1;
		  upstairs_Mode = Manual;
          camera_Flag = FORWARD;
        break;
        
        case GET_BULLET:
          mode = GET_BULLET;
          Lunar_Chassis.Set_SpeedGear(SLOW_GEAR);
          Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
          camera_Flag       = BULLET;
          Operations.get_bullet_flag = APPROACH;
          Operations.detected_flag   = 0;
        break;
        
        case GIVE_BULLET_NORMAL:
          //切换到慢速档并且降低加速度
          mode = GIVE_BULLET_NORMAL;
          Lunar_Chassis.Set_SpeedGear(SLOW_GEAR);
          Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
          break;
        
        case UP_STAIRS:
          mode = UP_STAIRS;
          Lunar_Chassis.Set_SpeedGear(NORMAL_GEAR);
          Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
          camera_Flag     = FORWARD;
          Operations.upstairs_flag = STEP1;
		  upstairs_Mode = Manual;
          break;
        
        case RESCUE:
          if(mode != RESCUE)
          {
            mode = RESCUE;
            Lunar_Chassis.Set_SpeedGear(NORMAL_GEAR);
            Lunar_Chassis.Set_TorqueOptimizeFlag(ON);
            camera_Flag  = BACK_RESCUE;
            Operations.rescue_flag = ATTACHING;
          }
          break;
        
        case AUTO_ROTATE:
          mode = AUTO_ROTATE;
          Lunar_Chassis.Update_ZeroPose();
          if(Operations.auto_rotate_flag == CAT_WALK && cat_walk_flag == 0)
			  cat_walk_flag = CAT_LEFT;	//先左旋
          break;
      }
      Lunar_Chassis.Switch_Mode(Normal_Speed);
  }
 }
 
void CEngineer_typedef::set_Target(float Manual_x,float Manual_y,float Manual_z)
 {
	Manual_Target.x = Manual_x;
	Manual_Target.y = Manual_y; 
	Manual_Target.z = Manual_z; 
 }
 /****Action****/
 
 /*每个action需要设置底盘模式、力矩优化、相机方向，速度挡位（一些模式可手动调节，
 则不用设，如NORMAL，RESCUE）；
 正常、救援、上岛在Gear_Box将遥控数据Set_Target，其他在action里Set_Target*/
 
 /*正常模式*/
 void CEngineer_typedef::action_Normal()
 {
	 Lunar_Chassis.Switch_Mode(Normal_Speed);
//	 Lunar_Chassis.Set_AttitudeOptimizeFlag(ON);
	 Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
 }
 
 /*取弹*/
 void CEngineer_typedef::action_Get_Bullet()
 {//待限制控制输出最大速度
//	Lunar_Chassis.Set_AttitudeOptimizeFlag(OFF);
  /* 
    取弹状态机
  */
  switch(Operations.get_bullet_flag)
  {
    //进入对准，可能未进入视觉控制范围
    case APPROACH:
      //Y轴距离1200内视觉自动切换,手动输入撤销后进入对准模式
      if(Operations.detected_flag == 1 && _Chassis_Abs(CV_Data.y) < 1200)
      {
        if(Manual_Target.x == 0 && Manual_Target.y == 0)
          Operations.get_bullet_flag = ALIGNING_FIRST;
        else
        {
          Lunar_Chassis.Switch_Mode(Follow_Position);
          Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
          Lunar_Chassis.Set_Target(CV_Data.x, CV_Data.y, -CV_Data.z);
        }//？
      }
      else
      {
        Lunar_Chassis.Switch_Mode(Normal_Speed);
        Lunar_Chassis.Set_SpeedGear(NORMAL_GEAR);
        Lunar_Chassis.Set_Target(Manual_Target.x, 0.8f*Manual_Target.y, Manual_Target.z);
        Lunar_Chassis.Set_TorqueOptimizeFlag(ON);
      }
    break;
    
    case MANUAL:
      Lunar_Chassis.Switch_Mode(Normal_Speed);
      Lunar_Chassis.Set_SpeedGear(SLOW_GEAR);
      Lunar_Chassis.Set_Target(Manual_Target.x,  0.5f*Manual_Target.y, Manual_Target.z);
      Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
      break;
    
    case ALIGNING_FIRST:
      Lunar_Chassis.Switch_Mode(Follow_Position);
      Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
      Lunar_Chassis.Set_Target(CV_Data.x, CV_Data.y, -CV_Data.z);
      //手动可打断
      if(Manual_Target.x != 0 || Manual_Target.y != 0)
        Operations.get_bullet_flag = MANUAL;
      break;
      
    case ALIGNING_AGAIN:
      Lunar_Chassis.Switch_Mode(Follow_Position);
      Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
      Lunar_Chassis.Set_Target(CV_Data.x, CV_Data.y, -CV_Data.z);
      //手动可打断
      if(Manual_Target.x != 0 || Manual_Target.y != 0)
        Operations.get_bullet_flag = MANUAL;
      break;
      
    default:
      break;
  }
  
  //全程深度视角
  camera_Flag = BULLET;
 }
 
 /*给弹（救援装置锁定？）*/
 void CEngineer_typedef::action_Give_Bullet()
 {
//	 Lunar_Chassis.Set_AttitudeOptimizeFlag(OFF);
	if(Lunar_Chassis.Get_Mode() != Normal_Speed)
	{
		Lunar_Chassis.Switch_Mode(Normal_Speed);
	}
	Lunar_Chassis.Set_SpeedGear(SLOW_GEAR);
	Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
 }
 
 /*上岛：被动上岛，一次上岛，*/
 void CEngineer_typedef::action_Upstairs()
 {
    Lunar_Chassis.Set_TorqueOptimizeFlag(OFF);
    Lunar_Chassis.Set_SpeedGear(NORMAL_GEAR);
//	Lunar_Chassis.Set_AttitudeOptimizeFlag(OFF);
    //模组视角
    camera_Flag = FORWARD; 	//待改
	switch(upstairs_Mode)
	{
		case Auto:
			Lunar_Chassis.Set_Target(-0 , - 0.9f, Manual_Target.z);
			break;
		case Manual:
			Lunar_Chassis.Set_Target(-0 , - Manual_Target.y, Manual_Target.z);
			break;
	}
    int flag = 0;
	if(IMU_Data.pitch > 10.0f)
	{
        flag = 1;
	 }
    if (flag == 1)
    {
        if (IMU_Data.pitch <= 5.0f)	//是否会一直卡住？
        {
            Lunar_Chassis.Set_Target(0, 0, 0);
            switch_Mode(NORMAL);
            flag = 0;
        }
    }
    //修改防翻车的逻辑结构！！！！！！！！！！！！！
 }
 
 /*救援（待增加卡片复活程序）*/
 void CEngineer_typedef::action_Rescue()
 {
	if(Lunar_Chassis.Get_Mode() != Normal_Speed)
	{
		Lunar_Chassis.Switch_Mode(Normal_Speed);
	}
	if(Operations.rescue_flag == ATTACHING)
	{
		camera_Flag = BACK_RESCUE;			//需要根据新车修改
       cout << "相机后转" << endl;
	}
	else if(Operations.rescue_flag == PULLING_OUT)
	{
		camera_Flag = FORWARD;
        cout << "相机前转" << endl;
	}
	//Lunar_Chassis.Set_TorqueOptimizeFlag(ON); 
 }
 
 /*自旋：单次，连续，猫步*/
 void CEngineer_typedef::action_Auto_Rotate()
 {
//	Lunar_Chassis.Set_AttitudeOptimizeFlag(OFF);
	if(Lunar_Chassis.Get_Mode() != Normal_Speed)
	{
		Lunar_Chassis.Switch_Mode(Normal_Speed);
	}
	Lunar_Chassis.Set_SpeedGear(FAST_GEAR);
	if(Operations.auto_rotate_flag == SINGLE)
	{
		Lunar_Chassis.Set_Target(0, 0, -1);
		if(Lunar_Chassis.Current_Pos.yaw - Lunar_Chassis.Zero_Pos.yaw > 144)	//待调参
		{
			  if(_Chassis_Abs(Lunar_Chassis.Current_Pos.yaw - Lunar_Chassis.Zero_Pos.yaw) > 180)
			  {
				Lunar_Chassis.Update_ZeroPose();
			  }
			  switch_Mode(NORMAL);
			  Lunar_Chassis.Set_Target(0, 0, 0);
		}		
	}
	else if(Operations.auto_rotate_flag == CONTINUOUS)
	{
		Lunar_Chassis.Set_Target(0, 0, 1);	
	}
	else if(Operations.auto_rotate_flag == CAT_WALK)
	{
		if(cat_walk_flag == 1)
		{
			Lunar_Chassis.Set_Target(0,0,-0.7f);
			if(Lunar_Chassis.Current_Pos.yaw - Lunar_Chassis.Zero_Pos.yaw > 25)	//待调参
			{
				if(Lunar_Chassis.Current_Pos.yaw - Lunar_Chassis.Zero_Pos.yaw > 180)	//转过角度大于180时更新零点位姿
				{
					Lunar_Chassis.Update_ZeroPose();
				}
				cat_walk_flag = 2;	
			}
		}
		if(cat_walk_flag == 2)
		{
			Lunar_Chassis.Set_Target(0,0,0.7f);
			if(Lunar_Chassis.Current_Pos.yaw - Lunar_Chassis.Zero_Pos.yaw < -25)
			{
				if(Lunar_Chassis.Current_Pos.yaw - Lunar_Chassis.Zero_Pos.yaw > 180)
				{
					Lunar_Chassis.Update_ZeroPose();
				}
				cat_walk_flag = 1;
			}
		}
		else 
		{
			Lunar_Chassis.Set_Target(0,0,0);
		}
	}

  if(Manual_Target.x != 0 || Manual_Target.y != 0 || Manual_Target.z != 0)
  {
    Lunar_Chassis.Set_Target(0,0,0);
    switch_Mode(NORMAL);
  }	
 }
 
 /**
* @brief  Speed up or Slow down under specific mode.
* @param  FAST_GEAR,NORMAL_GEAR,SLOW_GEAR
* @return None.
* @author 
*/
void CEngineer_typedef::GearBox(void)
{
  static bool gear_box_enable = 0;
  static float Transmit_Co = 0;
  
  /* Gear Box is only for manual input */  
  switch(mode)
  {
    case NORMAL:
      gear_box_enable = 1;
      if(camera_Flag == FORWARD)
        Transmit_Co = 1.0f;
      else
        Transmit_Co = -1.0f;
      break;
    
    case RESCUE:
      gear_box_enable = 1;
      if(camera_Flag == FORWARD)
        Transmit_Co = 1.0f;
      else
        Transmit_Co = -1.0f;
      break;
    
    case UP_STAIRS:
      if(upstairs_Mode == Manual)
      {
        gear_box_enable = 1;
        if(camera_Flag == FORWARD)
          Transmit_Co = 1.0f;
        else
          Transmit_Co = -1.0f;
      }
      else
        gear_box_enable = 0;
      break;

    case GIVE_BULLET_NORMAL:
        Transmit_Co = 1.0f;
        gear_box_enable = 1;
        break;

    default:
      gear_box_enable = 0;
      break;
  }
  
  if(gear_box_enable)
  {
    if(gear_flag == SPEED_UP)
      Lunar_Chassis.Set_SpeedGear(FAST_GEAR);
    else if(gear_flag == SLOW_DOWN)
      Lunar_Chassis.Set_SpeedGear(SLOW_GEAR);
    else 
      Lunar_Chassis.Set_SpeedGear(NORMAL_GEAR);

    Lunar_Chassis.Set_Target(Transmit_Co * Manual_Target.x, Transmit_Co * Manual_Target.y, Manual_Target.z);
  }
  
}

void CEngineer_typedef::set_AutoRotate(_Auto_Rotate_Status direction)
{
  if(direction != Operations.auto_rotate_flag)
    Lunar_Chassis.Update_ZeroPose();

  Operations.auto_rotate_flag = direction;
}
 /* -------------------------------------IO_Ports -----------------------------------------------*/
 /**
* @brief  I/O Stream of the robot.
          These functions is depended on the hardware of robot.
          If your robot is different, just rewrite I/O functions.
* @param  encoder_count,pose,Current(or Torque),Status,Power State
* @return int32_t
*/
void CEngineer_typedef::IO_Port(_Unit_Type UnitToOperate, _IO_Type mode, int32_t value)
{
  switch(mode)
  {
    /* Read in current wheel rpm */
    case RPM_IN:					//输入当前转速
      if(UnitToOperate < 6){
        if((UnitToOperate == WHEEL_RF) || (UnitToOperate == WHEEL_RB) || (UnitToOperate == WHEEL_RM))
          Lunar_Chassis.wheel_rpm[UnitToOperate] = -value;
        else
          Lunar_Chassis.wheel_rpm[UnitToOperate] = value;
      }
      break;
    
    /* Read in current torque(Current) */
    case TORQUE_IN:
      if(UnitToOperate < 6)
        Lunar_Chassis.wheel_torque[UnitToOperate] = value;
      break;
    case CONNECTION_STATE_IN:
      switch(UnitToOperate)
      {
       case BODY_MOTORS:
         all_Status.motor = value;
         break;
       case IMU:
         all_Status.IMU = value;
       case NUC:
         all_Status.NUC = value;
       case CHASSIS_MOTORS:
         all_Status.chassis_motors = value ;
        break;
      }
      break;
    
    /* Wrong call */
    default:
      break;
  }
}

void CEngineer_typedef::IO_Port(_Unit_Type UnitToOperate, _IO_Type mode, uint8_t value)
{
  switch(mode)
  {
//    case CHASSIS_MAGNET_IN:
//      if(UnitToOperate == CHASSIS_MAGNET)
//         all_Status.chassis_Magnet_Lift = value;
//    break;
//      
//    case CHASSIS_PES_IN:
//      if(UnitToOperate == CHASSIS_PES_PULL)
//        all_Status.chassis_PES_Pull = value;
//      else if(UnitToOperate == CHASSIS_PES_STOP)
//        all_Status.chassis_PES_Stop = value;
//    break;
//    
//    default:
//      break;
  }
}

int32_t CEngineer_typedef::IO_Port(_Unit_Type UnitToOperate, _IO_Type mode)
{
  switch(mode)
  {
    case WHEEL_OUT:
      if((UnitToOperate == WHEEL_LF) || (UnitToOperate == WHEEL_LB) || (UnitToOperate == WHEEL_LM))
        return (Lunar_Chassis.wheel_Out[UnitToOperate]);
      else
        return Lunar_Chassis.wheel_Out[UnitToOperate];
    
    /* Wrong call */
    default:
      break;
  }
  return 0;
}

void CEngineer_typedef::IO_Port(_IO_Type mode,float x,float y ,float z)
{
  if(mode == POSITION_IN)
  {
    CV_Data.x = x;
    CV_Data.y = y;
    CV_Data.z = z;
  }
  else if(mode == IMU_IN)
  {
    IMU_Data.roll = x;
    IMU_Data.pitch = y;
    IMU_Data.yaw = z;
  }
}

_Body_Operation_t* CEngineer_typedef::IO_Port(_IO_Type mode)
{
  if(mode == BODY_OPERATION_OUT)
    return &Operations;
  else 
    return NULL;
}
 
 /* function prototypes -------------------------------------------------------*/

