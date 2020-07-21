/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    BMX055.h
  * @author  YDX 2244907035@qq.com
  * @brief   Code for BMX055
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
#ifndef _BMX055_H_
#define _BMX055_H_

#ifdef __cplusplus
extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/ 
#include "Drivers/Components/drv_i2c.h"
#include "Attitude_Processing.h"
/* Private macros ------------------------------------------------------------*/       
#define IIC_BMX055_ACC_ADR    0x18
#define IIC_BMX055_GYRO_ADR   0x68
#define IIC_BMX055_MAG_ADR    0x10   

#define BMX055_ACC_XDATALSB   0x02
#define BMX055_ACC_ID         0x00
#define BMX055_ACC_PMURANGE   0x0F
#define BMX055_ACC_PMUBW      0x10
#define BMX055_ACC_PMULPM     0x11


#define BMX055_GYRO_XDATALSB  0x02
#define BMX055_GYRO_ID        0x00
#define BMX055_GYRO_RANGE     0x0F
#define BMX055_GYRO_BW        0x10
#define BMX055_GYRO_LPM       0x11
#define BMX055_GYRO_RATEHBW   0x13

#define BMX055_MAG_XDATALSB   0x42
#define BMX055_MAG_ID         0x40
#define BMX055_MAG_POM        0x4B
#define BMX055_MAG_DATARATE   0x4C
#define BMX055_MAG_INTEN      0x4E

/* Private type --------------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  float GYROXdata;
  float GYROYdata;
  float GYROZdata;
  float ACCXdata;
  float ACCYdata;
  float ACCZdata;
  float MAGXdata;
  float MAGYdata;
  float MAGZdata;
}BMX055Datatypedef;

/* Exported variables --------------------------------------------------------*/
extern BMX055Datatypedef BMX055_data;

/* Exported function declarations --------------------------------------------*/
uint8_t BMX_Conf(IIC_HandleTypeDef *hiic);
uint8_t BMX055_DataRead(IIC_HandleTypeDef *hiic, BMX055Datatypedef *Q, uint8_t type);

uint8_t BMX055_get_angle(float *roll, float *pitch, float *yaw);
uint8_t BMX055_get_angleSpeed(float *roll, float *pitch, float *yaw);
#ifdef __cplusplus
}
#endif

#endif
/************************ COPYRIGHT SCUT-ROBOTLAB *****END OF FILE*************/

