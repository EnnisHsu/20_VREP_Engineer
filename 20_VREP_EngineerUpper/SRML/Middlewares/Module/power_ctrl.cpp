/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    PowerCtrl.cpp
  * @author  Kainan.Su 15013073869
  * @brief   Power control for chassis in Robomaster.
  * @date    2019-11-12
  * @version 1.0
  * @par Change Log：
  * <table>
  * <tr><th>Date        	<th>Version  <th>Author    		<th>Description
  * <tr><td>2019-10-29   	<td> 1.0     <td>Kainan    		<td>实现充电开关，以及电机的限幅输出
  * <tr><td>2019-11-24   	<td> 2.0     <td>Kainan    		<td>增加构造函数，控制器外部实现，RF供电和CAP供电模式	\n
																														加入功率限制开关和电容充电计算开关
  * <tr><td>2019-12-22   	<td> 2.1     <td>Kainan    		<td>增加RF供电模式下电机和充电功率的计算	\n
  * </table>
  *
  ==============================================================================
                            How to use this Module  
  ==============================================================================
    @note 
			-# 新建"powerCtrl"对象，构造时设置初始参数，选择是否进行限幅和充电计算. \n
      加载外部的控制器Load_xxxxController().  
			
      -# 按照配置的运行频率调用 Control().通过Set_xx()设置参数、目标
			
      -# 调用Get_LimScale()获得限幅比例,调用Get_ChargePower()获得允许充电功率
			

		@attention 
			-# 请仔细阅读ReadMe.md
			
			-# 如果一些测试机构不需要用到功率控制，不需要改变其他的内容， \n 
				 只需要在构造函数中设置DISABLE就可以了，此时当读取电机限幅比例会一直都是1.0，相当于不不限幅。
      
			-# 待完善
				 RF供电下的控制		
    @example
			

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

/* Includes ------------------------------------------------------------------*/
#include "power_ctrl.h"
#include <stddef.h>	/* 使用NULL */

/* Private define ------------------------------------------------------------*/
#define myabs(x) ((x)>0? (x):(-(x)))

/* Private variables ---------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief Load external controller
 * @note 
 * @param None
 * @retval None
 * @author kainan
 */
void PowerCtrl_ClassDef::Load_CapChargeController(float(*pFunc)(const float current, const float target))
{
	CapChargeController = pFunc;
}

void PowerCtrl_ClassDef::Load_MotorLimit_CAP_Controller(float(*pFunc)(const float current, const float target))
{
	MotorLimit_CAP_Controller = pFunc;
}

void PowerCtrl_ClassDef::Load_MotorLimit_RF_Controller(float(*pFunc)(const float current, const float target))
{
	MotorLimit_RF_Controller = pFunc;
}

/**
  * @brief  功率计算的主函数
  * @param  None
  * @retval None
  * @author 苏锴南
  */ 
void PowerCtrl_ClassDef::Control(float _RF_P,float _motor_P, float _remainEnergy, int *MotorOut)
{
	Update(_RF_P,_motor_P, _remainEnergy);
	
	if(limit_EN == ENABLE_power)
		IsLimit = Calc_motorLimit(MotorOut);
	else
		limScale = 1.0f;

	
	if(capCharge_EN == ENABLE_power)
		Calc_capChargePower();
	else
		capChargePower = 0;
}

/**
 * @brief  得到电容充电的最大功率
 * @param  None
 * @retval None
 * @author 苏锴南
 */
float PowerCtrl_ClassDef::Get_ChargePower(void)
{
	return capChargePower;
}

/**
 * @brief  得到电机输出限幅比例
 * @param  None
 * @retval None
 * @author 苏锴南
 */
float PowerCtrl_ClassDef::Get_LimScale(void)
{
	return limScale;
}


/**
 * @brief  计算电机是否功率限制，并在内部得到限幅值，返回电机是否收到功率限制
 * @note	 传入电机电流环的输出值，并把返回值赋值给IsLimit
 * @param  电机电流环的输出值 
 * @retval 电机是否收到功率限制
 * @author 苏锴南
 */

IsLimit_e PowerCtrl_ClassDef::Calc_motorLimit(int *MotorOut)
{
	float Limitation = 0;	
	
	if(Mode == CAP_power)
	{	
		if(MotorLimit_CAP_Controller != NULL)
			Limitation = motorPowerMax*10 + MotorLimit_CAP_Controller(motorPower, motorPowerMax);
	}
	else
	{
		if(MotorLimit_RF_Controller != NULL)		
			Limitation = RFPowerMax*10 + MotorLimit_RF_Controller(remainEnergy, remainEnergyMax);			
	}
	
	/* 如果不加限幅，当超功率时，limitation负大进行功率限制，scale也负大 */
	if(Limitation <= 300)	
		Limitation = 300;
	if(Limitation >= maxCurrent)	
		Limitation = maxCurrent;
		
	float Scale=1.0f;
	float Current_Max=0.0f;
	
	/* 取最大输出 */
	for(uint8_t i=0;i<=3;i++)		
		if(myabs(MotorOut[i])>=Current_Max)
			Current_Max=myabs(MotorOut[i]);		
	/* 如果超功率就进行功率限制 */
	if(Current_Max>Limitation)
		Scale=Limitation/Current_Max;	
	
	limScale = Scale;
				
	if(Scale != 1.0f)
		return IsLimit_YES;
	else
		return IsLimit_NO;
}


/**
 * @brief  计算电容充电功率
 * @param  None
 * @retval None
 * @author 苏锴南
 */
void PowerCtrl_ClassDef::Calc_capChargePower(void)
{
	/* 电池供电下判断底盘是否受到功率限制来判断是否给电容充电 */
	if(CapChargeController != NULL)		
	{
		if(Mode == RF_power)
		{
			if(IsLimit == IsLimit_NO)
			{
				capChargePower = RFPowerMax - RFPower - CapChargeController(remainEnergy, remainEnergyMax); 	
				if(capChargePower < 0)
					capChargePower = 0;
				else{}			
			}
			else
			{
				/* PID添加清零 */
				capChargePower = 0;
			}
		}
		/* 电容供电下电池给电容充电 */
		else if(Mode == CAP_power)
		{	
			capChargePower = RFPowerMax - CapChargeController(remainEnergy, remainEnergyMax); 	
			if(capChargePower < 0)
				capChargePower = 0;
			else{}
		}		
	}
}

/**
 * @brief  更新功率数据的值
 * @param  电池功率，电机功率，RF剩余能量
 * @retval None
 * @author 苏锴南
 */
void PowerCtrl_ClassDef::Update(float _RFPower,float _motorPower, float _remainEnergy)
{
	static float Last_RF_Remain_Q = 0;
	RFPower = _RFPower;
	motorPower = _motorPower;
	
	/* 裁判系统的数据是否更新 */
	if(_remainEnergy != Last_RF_Remain_Q)
	{		
		remainEnergy = _remainEnergy;
		Last_RF_Remain_Q = remainEnergy;
	}
	else
		remainEnergy += (RFPowerMax - RFPower) * ctrlPeriod / 1000.0f;
	if(remainEnergy >= 60)	
		remainEnergy = 60.0f;
	else{}
}

/**
 * @brief  设置电池功率，电机功率，剩余能量的最大值
 * @param  电池功率，电机功率，剩余能量的最大值
 * @retval None
 * @author 苏锴南
 */
void PowerCtrl_ClassDef::Set_PE_Max(float _RFPowerMax, float _motorPowerMax, float _remainEnergyMax)
{
	RFPowerMax = _RFPowerMax;
	motorPowerMax = _motorPowerMax;
	remainEnergyMax = _remainEnergyMax;
}

/**
 * @brief  设置电池供电还是电容供电(影响内部计算，不影响物理开关)
 * @note   (影响内部计算，不影响物理开关)
 * @param  电池功率，电机功率，剩余能量的最大值
 * @retval None
 * @author 苏锴南
 */
void PowerCtrl_ClassDef::Set_Mode(SourceMode_e _Mode)
{
	Mode = _Mode;
}

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
