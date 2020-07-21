/**
  ******************************************************************************
 * Copyright (c) 2020 - ~, RobotLab Development Team
  * @file   ：Engineer.h
  * @brief  ：Header of Engineer.cpp
  * @version：0.0.1
  * @changes：
 *  Date           Author   Version    Notes
 *  2019-11-10     Forsea    0.0.1     first version
  ******************************************************************************
  */

#ifndef _ENGINEER_H
#define _ENGINEER_H 

 /* Includes ------------------------------------------------------------------*/
#include "chassis.h"
#include "stdint.h"
#include "mycontroller.h"

 #ifdef __cplusplus
  extern "C" {
 #endif

 /* Private macros ------------------------------------------------------------*/

#define		ONLINE		1	  
#define		OFFLINE		0
#define 	CAT_WAIT	0
#define 	CAT_LEFT	1
#define 	CAT_RIGHT	2
#define	 	OFF         0
#define 	ON          1	  
enum _ActionMode_t
{
  CHASSIS_LOCK = 0U,
  NORMAL,
  GET_BULLET,
  GIVE_BULLET_NORMAL,
  UP_STAIRS,
  RESCUE,
  AUTO_ROTATE,
};

typedef struct CV_data
{
    float Pos_x;
    float Pos_y;
    float Pos_z;
}CV_data;

enum _Unit_Type //IO_Port中协助区分数据
{
  WHEEL_LF = 0U,
  WHEEL_RF,
  WHEEL_RB,
  WHEEL_LB,
  WHEEL_LM,
  WHEEL_RM,
  CHASSIS_MOTORS,
  BODY_MOTORS,
  CHASSIS_PES_PULL,
  CHASSIS_PES_STOP,
  CHASSIS_MAGNET,
  IMU,
  NUC
};

enum _IO_Type
{
  WHEEL_OUT = 0U,
  RPM_IN,
  POSITION_IN,
  IMU_IN,
  TORQUE_IN,
  CHASSIS_TEMPRE_IN,
  CHASSIS_MAGNET_IN,
  CHASSIS_PES_IN,
  CONNECTION_STATE_IN,
  BODY_OPERATION_OUT
};

/*需要根据新车结构更改*/
enum _Camera_Status 
{
  FORWARD = 0U,
  BACK_RESCUE,
  BULLET
};

enum _GetBullet_Status
{
  APPROACH = 0U,
  MANUAL,
  ALIGNING_FIRST,
  ALIGNING_AGAIN
};

enum _UpStairs_Status
{
  STEP1 = 0U,
  STEP2,
  LAND_ON
};
enum _UpStairs_Mode
{
  Auto = 0U,
  Manual
};

enum _RobotStatus_t
{
  Power_Disable = 0U,
  Power_Enable,
  Robot_Fault
};

enum _Rescue_Status
{
  ATTACHING = 0U,
  PULLING_OUT
};

enum _Auto_Rotate_Status
{
  SINGLE = 0U,
  CONTINUOUS,
  CAT_WALK
};

typedef struct _Total_Status	
{
	uint8_t chassis_motors;	//与Motor_C620相联系
	uint8_t motor;		
	bool 	NUC;			//ONLINE or OFFLINE
	bool 	IMU;
	bool	MOVE;
	bool 	LIFT;	
}_Engineer_Status;

typedef struct _Body_Operation_t
{
  _GetBullet_Status   	get_bullet_flag;
  uint8_t        		detected_flag;
  uint8_t        		send_bullet_flag;
  _Rescue_Status     	rescue_flag;
  _UpStairs_Status    	upstairs_flag;
  _Auto_Rotate_Status 	auto_rotate_flag;
}_Body_Operation_Status;

typedef struct Euler_3f
{
  float roll;
  float pitch;
  float yaw;
}Euler_3f;

typedef struct Vector_3f
{
  float x;
  float y;
  float z;
}Vector_3f;

enum _Gear_t
{
  NO_CHANGE = 0U,
  SPEED_UP,
  SLOW_DOWN
};

class CEngineer_typedef
{
private:
	_ActionMode_t 			mode;	
	_Engineer_Status 		all_Status;
	uint8_t 				            cat_walk_flag;
	Euler_3f			                IMU_Data;
	Vector_3f				        Manual_Target;
	Vector_3f			            CV_Data;

public:
	_UpStairs_Mode			            upstairs_Mode;
	_Gear_t 				                        gear_flag;
	_Body_Operation_Status      Operations;	//各种过程的状态机
	_Camera_Status 			            camera_Flag;

    CEngineer_typedef() {};
	CChassis Lunar_Chassis = CChassis(461.0f,600.0f,76.0f,96,73,1,0.005f);
	void Init();
	void switch_Mode(_ActionMode_t targetMode);
	_ActionMode_t get_CurrentMode();
	void center_Control();
	void GearBox(void);
	void set_Target(float Manual_x,float Manual_y,float Manual_z);	
	void set_AutoRotate(_Auto_Rotate_Status direction);
		
	void action_Normal();
	void action_Get_Bullet();
    void action_Give_Bullet();
    void action_Upstairs();
    void action_Rescue();
    void action_Auto_Rotate();
	
    int32_t  IO_Port(_Unit_Type UnitToOperate, _IO_Type mode);
    void     IO_Port(_Unit_Type UnitToOperate, _IO_Type mode, int32_t value);
    void     IO_Port(_Unit_Type UnitToOperate, _IO_Type mode, uint8_t value);
    void     IO_Port(_IO_Type mode, float x,float y ,float z);
    _Body_Operation_t* IO_Port(_IO_Type mode);
};
 /* Private type --------------------------------------------------------------*/

 /* Exported macros -----------------------------------------------------------*/
	 
 /* Exported types ------------------------------------------------------------*/
 
 /* Exported function declarations --------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /*_ENGINEER_H*/

