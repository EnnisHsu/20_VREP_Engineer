/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    BMX055_Config.h
  * @author  YDX 2244907035@qq.com
  * @brief   
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
#ifndef _BMX055_CONFIG_H_
#define _BMX055_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/  
#include "BMX055.h"
#include "Attitude_Processing.h"
/* Private macros ------------------------------------------------------------*/
/* Private type --------------------------------------------------------------*/
void IIC_SCL_B(const char x);
void IIC_SDA_B(const char x);
void SDA_IN_B(void);
void SDA_OUT_B(void);
GPIO_PinState READ_SDA_B(void);
  
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct 
{
	GPIO_TypeDef * BMX055_IIC_GPIO_PORT;
	uint32_t BMX055_IIC_SCL_PIN;
	uint32_t BMX055_IIC_SDA_PIN;
	uint32_t BMX055_IIC_SCL_PIN_NUM;
	uint32_t BMX055_IIC_SDA_PIN_NUM;	
}BMX055_IIC_PIN_Typedef;

/* Exported variables --------------------------------------------------------*/
extern IIC_HandleTypeDef BMX055_IIC;	
extern BMX055_IIC_PIN_Typedef BMX055_IIC_PIN;

/* Exported function declarations --------------------------------------------*/
void BMX055_Init(GPIO_TypeDef* gpiox,uint32_t scl_pinx,uint32_t sda_pinx,uint32_t scl_pin_numx,uint32_t sda_pin_numx,float cal_period);

#ifdef __cplusplus
}
#endif

#endif

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
