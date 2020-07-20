/**
******************************************************************************
* Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
* @file    power_ctrl.h
* @author  Kainan.Su 15013073869
* @brief   Header file of power_ctrl.
******************************************************************************
* @attention
* 
* if you had modified this file, please make sure your code does not have many 
* bugs, update the version Number, write dowm your name and the date, the most
* important is make sure the users will have clear and definite understanding 
* through your new brief.
*
* <h2><center>&copy; Copyright (c) 2019 - ~, SCUT-RobotLab Development Team.
* All rights reserved.</center></h2>
******************************************************************************
*/

#ifndef __POWERCTRL_H_
#define __POWERCTRL_H_
#ifdef __cplusplus
/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Private macros ------------------------------------------------------------*/
/* Private type --------------------------------------------------------------*/
typedef enum IsLimit_Enum_power
{
	IsLimit_NO,
	IsLimit_YES
}IsLimit_e;

typedef enum ENABLE_Enum_power
{
	DISABLE_power,
	ENABLE_power	
}ENABLE_e;

typedef enum SourceMode_Enum_power
{
	CAP_power,	
	RF_power		/* 使用剩余能量进行电机功率控制 */
}SourceMode_e;

/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
class PowerCtrl_ClassDef
{
  private:
   	uint8_t ctrlPeriod;					/*<! 控制周期 单位ms */
	float maxCurrent;					/*<! 最大电流输出值 */
	ENABLE_e capCharge_EN;     		/*<! 电容充电开关 */
	ENABLE_e limit_EN;				/*<! 功率限制开关 */
	SourceMode_e Mode;				/*<! 电容/RF裁判系统供电模式 */
  
	IsLimit_e IsLimit;	    /*<! 底盘是否收到功率限制 */
    float capChargePower;		  /*<! 电容充电功率 */		
    float limScale;						/*<! 电机限幅的比例*/

	float(*CapChargeController)(const float current, const float target);
	float(*MotorLimit_CAP_Controller)(const float current, const float target);
	float(*MotorLimit_RF_Controller)(const float current, const float target);
	
	void Update(float _RF_P,float _motor_P, float _remainEnergy);
    IsLimit_e Calc_motorLimit(int *MotorOut);	
    void Calc_capChargePower();

  public: 
	PowerCtrl_ClassDef(SourceMode_e _Mode, ENABLE_e _limit_EN, uint8_t _ctrlPeriod = 1, float _maxCurrent = 35000, ENABLE_e _capCharge_EN = ENABLE_power
						){
		ctrlPeriod = _ctrlPeriod;
		maxCurrent = _maxCurrent;			
		capCharge_EN = _capCharge_EN;
							
		Mode = _Mode;
		limit_EN =_limit_EN;					
	}
		
	/*<! 功率相关变量 为了方便调试 放在外面 */
	float RFPower;
	float motorPower;
	float remainEnergy;
	float motorPowerMax;
	float RFPowerMax;
	float remainEnergyMax;	

	
    void Load_CapChargeController(float(*pFunc)(const float current, const float target));
    void Load_MotorLimit_CAP_Controller(float(*pFunc)(const float current, const float target));		
    void Load_MotorLimit_RF_Controller(float(*pFunc)(const float current, const float target));		

	/* 主函数 */
    void Control(float _RF_P,float _motor_P, float _remainEnergy, int *MotorOut);
    
    /* 设定参数 */
    void Set_PE_Max(float _RF_P_Max, float _motor_P_Max, float _Q_Max);
    void Set_Mode(SourceMode_e _Mode);
		
    /* 获得电容充电供电 */    
    float Get_ChargePower(void);
		
    /* 获得功率控制值 */
	float Get_LimScale(void);

};	
#endif
#endif
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
