/**
******************************************************************************
* Copyright (c) 2020 - ~, SCUT-RobotLab Development Team
* @file    Motor_C620.cpp
* @author  Forsea
* @brief   初始化电机实例，该库仅用于储存底盘电机的当前状态，将电机控制数据转换为CAN数据包结构。
* @date    2019-11-09
* @version 1.0
* @par Change Log：
* <table>
* <tr><th>Date        <th>Version  <th>Author    		<th>Description
* <tr><td>2019-11-09  <td> 1.0     <td>Forsea             <td>Creator
* <tr><td>2019-12-08  <td> 1.1     <td>Forsea             <td>Debug
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
#include "Motor_C620.h"
 
 /* Private define ------------------------------------------------------------*/
    
 /* Private variables ---------------------------------------------------------*/
   
 /* Private type --------------------------------------------------------------*/

 /* Private function declarations ---------------------------------------------*/
 
 /**
 *@brief	用于将PID输出的电机转速生成CAN数据结构体
 *@note		low用于id为1~4的电机，high用于id为5~8的电机,需要多次更新
 *@param	id:对应电机的id（1~8）；wheel_out:将要发送给电机的转速
 *@retval	CAN数据结构体
 *@author	Forsea
 */
 
void Motor_Ctrl::can_tx_update(uint8_t motor_id,int32_t wheel_out)
 {
	 if(motor_id == 2 |motor_id == 3|motor_id == 6)
	 {
		Tx_buff[motor_id*2 -2] = -wheel_out;
	 }
	else 
		Tx_buff[motor_id*2 - 2] = wheel_out;
 }
 
/**
 *@brief	获取各电机的当前状态
 *@note
 *@param	motor_id:对应电机的id（1~8）；
 *@retval	对应电机的当前状态
 *@author	Forsea
 */
 int16_t Motor_Ctrl::getSpeed(uint8_t motor_id)
 {
     return (int16_t)Rx_buff[motor_id - 1];
 }
 
 int16_t Motor_Ctrl::getAngle(uint8_t motor_id)
 {
//	return Motor_List[motor_id - 1].getAngle();
     return 0;
 }
 
 /**
 *@brief	将CAN接收到的数据更新到电机
 *@note
 *@param 	CAN接收数据包
 *@retval	NULL
 *@author	Forsea
 */
 
 void Motor_Ctrl::updata_all(int16_t* rx_buff)
 {
	for(uint8_t index = 0;index <= 7;index++)
	{
        memcpy(Rx_buff, rx_buff, 12);
	}
 }
 
 /**
 *@brief	检测电机是否正常连接
 *@note
 *@param	待检测的电机id（1~8）
 *@retval	ONLINE(连接正常)；OFFLINE(连接失败)
 *@author	Forsea
 */
 uint8_t Motor_Ctrl::Is_Connect(uint8_t motor_id)
 {
//	if(Motor_List[motor_id-1].getTempature() != 0)
//	{
//		return ONLINE;
//	}
//	else 
		return OFFLINE;
 }
 
 int16_t* Motor_Ctrl::get_Tx_buff()
 {
	return Tx_buff;
 }

 /* function prototypes -------------------------------------------------------*/
 