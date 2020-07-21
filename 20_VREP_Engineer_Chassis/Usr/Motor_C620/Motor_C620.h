/**
  ******************************************************************************
 * Copyright (c) 2020 - ~, RobotLab Development Team
  * @file   ：Motor_C620.h
  * @brief  ：Header of Motor_C620.cpp
  * @version：0.0.1
  * @changes：
 *  Date           Author   Version    Notes
 *  2019-11-1     Forsea    0.0.1     first version
 *  2019-12-08    Forsea    0.0.2     Debug
  ******************************************************************************
  */

#ifndef _MOTOR_C620_H
#define _MOTOR_C620_H
 /* Includes ------------------------------------------------------------------*/
#include "System_DataPool.h"
 #ifdef __cplusplus
  extern "C" {
 #endif
	  
 /* Private macros ------------------------------------------------------------*/
#define		ID_LF	1
#define		ID_RF	2
#define		ID_RB	3
#define		ID_LB	4
#define		ID_LM	5
#define		ID_RM	6  		

#define		ONLINE	1	  
#define		OFFLINE	0
	  
//typedef struct{
//  uint16_t  ID;
//  uint8_t   DLC;
//  uint8_t   Data[8];
//}COB_TypeDef;	//若有重复定义可注释掉

class Motor_Ctrl
{
private:
	//Motor_C620 Motor_List[8] = {Motor_C620(1),Motor_C620(2),Motor_C620(3),Motor_C620(4),Motor_C620(5),Motor_C620(6),Motor_C620(7),Motor_C620(8)}; 
	int16_t Tx_buff[6];
	int16_t Rx_buff[6];
public:
	void updata_all(int16_t* Rx_buff);
	void can_tx_update(uint8_t motor_id,int32_t wheel_out);   //id:1~4
	int16_t* get_Tx_buff();
	int16_t getSpeed(uint8_t motor_id);
	int16_t getAngle(uint8_t motor_id);
	uint8_t Is_Connect(uint8_t motor_id);
};
 /* Private type --------------------------------------------------------------*/
 


 /* Exported macros -----------------------------------------------------------*/
	 
 /* Exported types ------------------------------------------------------------*/
 
 /* Exported function declarations --------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /*_MOTOR_C620_H*/
