/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    drv_i2c.h
  * @author  YDX 
  * @brief   Code for iic driver in STM32 series MCU, supported packaged:
  *          - STM32Cube_FW_F4_V1.24.0.
  *          - STM32Cube_FW_F1_V1.8.0.
  *          - STM32Cube_FW_H7_V1.5.0.
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
#ifndef _DRV_I2C_H
#define _DRV_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#if defined(STM32F405xx) && defined(USE_HAL_DRIVER)
  #include <stm32f4xx_hal.h>
#endif
#if defined(STM32F103xx) && defined(USE_HAL_DRIVER)
  #include <stm32f1xx_hal.h>
#endif
#if defined(STM32H750xx) && defined(USE_HAL_DRIVER)
  #include <stm32h7xx_hal.h>
#endif	
/* Private macros ------------------------------------------------------------*/
/** 
* @brief  subject to the hardware circuit, user should set the communication line of I2C as small as possible, 
*         user can decrease this time to decrease the time of reading data 
*/
#define IIC_Delay_Time 20	
	
/* Private type --------------------------------------------------------------*/
/** 
* @brief  iic control operation
*/
typedef struct 
{
	GPIO_PinState (* xREAD_SDA)(void);      /*<! read SDA */
	void (* xSDA_IN)();              		    /*<! SDA input config */
	void (* xSDA_OUT)();             		    /*<! SDA output config */
	void (* xIIC_SCL)(const char x);        /*<! SCL output level */
	void (* xIIC_SDA)(const char x);        /*<! SDA output level */
}IIC_HandleTypeDef;

/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function declarations --------------------------------------------*/	
void IIC_Delay(uint8_t m);
void IIC_Init(IIC_HandleTypeDef *hiic);                                           
void IIC_Start(IIC_HandleTypeDef *hiic);				                  
void IIC_Stop(IIC_HandleTypeDef *hiic);	  			                  
void IIC_Send_Byte(IIC_HandleTypeDef *hiic,unsigned char txd);			   
unsigned char IIC_Read_Byte(IIC_HandleTypeDef *hiic,unsigned char ack); 
unsigned char IIC_Wait_Ack(IIC_HandleTypeDef *hiic); 				    
void IIC_Ack(IIC_HandleTypeDef *hiic);					                  
void IIC_NAck(IIC_HandleTypeDef *hiic);				                 
unsigned char IIC_Device_Write_Byte(IIC_HandleTypeDef *hiic, unsigned char addr, unsigned char reg, unsigned char data);
unsigned char IIC_Device_Read_Byte(IIC_HandleTypeDef *hiic,unsigned char addr,unsigned char reg);
unsigned char IIC_Device_Write_Len(IIC_HandleTypeDef *hiic, unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf);
unsigned char IIC_Device_Read_Len(IIC_HandleTypeDef *hiic, unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf);

#ifdef __cplusplus
}
#endif

#endif

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
