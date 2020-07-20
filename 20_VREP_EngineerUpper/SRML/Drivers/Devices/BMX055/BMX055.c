/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    BMX055.c
  * @author  YDX 2244907035@qq.com
  * @brief   Code for BMX055.
  * @date    2019-11-21
  * @version 1.0
  * @par Change Log:
  * <table>
  * <tr><th>Date        <th>Version  <th>Author  <th>Description
  * <tr><td>2019-11-21  <td> 1.0     <td>YDX     <td>Creator
  * </table>
  ******************************************************************************
    @warning	
      -# 使用说明在`BMX055_Config.C`
      -# 不要修改本文件，修改联系管理员！！！！！！！！！！！
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
#include "Attitude_Calculation.h"
#include "BMX055.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
BMX055Datatypedef BMX055_data;

/* Private type --------------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  BMX055 Config
  * @param  hiic: handle of iic
  * @retval 0,successful
  *         1,fail        
  */
uint8_t BMX_Conf(IIC_HandleTypeDef *hiic)
{
  /* accelerometer config */
  uint8_t ErrCount = 0;
	
  /* initialize iic bus */
  IIC_Init(hiic);
	
  /* confirm accelerometer chip ID */
  while(IIC_Device_Read_Byte(hiic, IIC_BMX055_ACC_ADR, BMX055_ACC_ID) != 0xFA)   
  {
    ErrCount++;
    if(ErrCount > 5)
      return 1;
  }
  
  /* accelerometer range config */
  if(IIC_Device_Write_Byte(hiic, IIC_BMX055_ACC_ADR, BMX055_ACC_PMURANGE, 0x05) == 1)return 1;   //4G
  HAL_Delay(10);
  
  /* accelerometer output rate config */
  if(IIC_Device_Write_Byte(hiic, IIC_BMX055_ACC_ADR, BMX055_ACC_PMUBW, 0x0F) == 1)return 1;     //1000HZ      
  HAL_Delay(10);
  
  /* accelerometer mode config */
  if(IIC_Device_Write_Byte(hiic, IIC_BMX055_ACC_ADR, BMX055_ACC_PMULPM, 0x00) == 1)return 1;   //Normal MODE      
  HAL_Delay(10);
  
  /* gyroscope config */
  ErrCount = 0;
  
  /* confirm gyroscope chip ID */
  while(IIC_Device_Read_Byte(hiic, IIC_BMX055_GYRO_ADR, BMX055_GYRO_ID) != 0x0F)   
  {
    ErrCount++;
    if(ErrCount > 5)
      return 1;
  }
  
  /* gyroscope range config */
  if(IIC_Device_Write_Byte(hiic, IIC_BMX055_GYRO_ADR, BMX055_GYRO_RANGE, 0x00) == 1)return 1;   //+-2000dps      
  HAL_Delay(10);
  
  /* gyroscope output rate config */
  if(IIC_Device_Write_Byte(hiic, IIC_BMX055_GYRO_ADR, BMX055_GYRO_BW, 0x02) == 1)return 1;     //1000HZ      
  HAL_Delay(10);
  
  /* gyroscope mode config */
  if(IIC_Device_Write_Byte(hiic, IIC_BMX055_GYRO_ADR, BMX055_GYRO_LPM, 0x00) == 1)return 1;   //Normal MODE      
  HAL_Delay(10);
  
  /* gyroscope high-pass filter config */  
  if(IIC_Device_Write_Byte(hiic,IIC_BMX055_GYRO_ADR, BMX055_GYRO_RATEHBW, 0x08) == 1)return 1;   //not necessary 
  HAL_Delay(10);
  
  /* magnetometer config */
  ErrCount = 0;
  
  /* awake magnetometer */   
  IIC_Device_Write_Byte(hiic, IIC_BMX055_MAG_ADR, BMX055_MAG_POM, 0x81);
  HAL_Delay(10);
  
  /* confirm magnetometer chip ID */  
  while(IIC_Device_Read_Byte(hiic, IIC_BMX055_MAG_ADR, BMX055_MAG_ID) != 0x32)   
  {
    ErrCount++;
    if(ErrCount > 5)
      return 1;
  }

  /* magnetometer output rate config */  
  if(IIC_Device_Write_Byte(hiic, IIC_BMX055_MAG_ADR, BMX055_MAG_DATARATE, 0x38) == 1)return 1;   //max speed 30HZ       
  HAL_Delay(10);
  
  /* magnetometer interrupt mode config */  
  if(IIC_Device_Write_Byte(hiic, IIC_BMX055_MAG_ADR, BMX055_MAG_INTEN, 0x00) == 1)return 1;      //disable interrupt    
  HAL_Delay(10);
  return 0;
}

/**
  * @brief  read BMX055 data
  * @param  hiic: handle of iic
  * @param  Q: struct for storing bmx055 data 
  * @param  type: 0 read acc and gyro data
  *               1 read acc,gyro and mag data
  * @retval 0,success
  *         1,fail    
  */
uint8_t BMX055_DataRead(IIC_HandleTypeDef *hiic, BMX055Datatypedef *Q, uint8_t type)
{
  uint8_t datatemp[6] = {0};
  
  /* read accelerometer data*/
  if(IIC_Device_Read_Len(hiic, IIC_BMX055_GYRO_ADR, BMX055_GYRO_XDATALSB, 6, datatemp) == 1)return 1;
  Q->GYROXdata = (float)((int16_t)((datatemp[1] << 8) | datatemp[0]));
  Q->GYROYdata = (float)((int16_t)((datatemp[3] << 8) | datatemp[2]));
  Q->GYROZdata = (float)((int16_t)((datatemp[5] << 8) | datatemp[4]));
  
  /* read gyroscope data*/  
  if(IIC_Device_Read_Len(hiic, IIC_BMX055_ACC_ADR, BMX055_ACC_XDATALSB, 6, datatemp) == 1)return 1;
  Q->ACCXdata = (float)((int16_t)((datatemp[1] << 8) | datatemp[0]) >> 4);
  Q->ACCYdata = (float)((int16_t)((datatemp[3] << 8) | datatemp[2]) >> 4);
  Q->ACCZdata = (float)((int16_t)((datatemp[5] << 8) | datatemp[4]) >> 4);

  /* read magnetometer data*/
  if(type)
  {
    if(IIC_Device_Read_Len(hiic, IIC_BMX055_MAG_ADR, BMX055_MAG_XDATALSB, 6, datatemp) == 1)return 1;
    Q->MAGXdata = (float)((int16_t)((datatemp[1] << 8) | datatemp[0]) >> 3);
    Q->MAGYdata = (float)((int16_t)((datatemp[3] << 8) | datatemp[2]) >> 3);
    Q->MAGZdata = (float)((int16_t)((datatemp[5] << 8) | datatemp[4]) >> 1);
  }
  return 0;
}

/**
 * @brief  Get the euler angle from BMX055. 
 * @param  roll, pitch, yaw
 * @retval None
 */
uint8_t BMX055_get_angle(float *roll, float *pitch, float *yaw)
{
  *roll = SystemAttitude.Roll;
  *pitch = SystemAttitude.Pitch;
  *yaw = SystemAttitude.Yaw;
  
  return 1;
}

/**
 * @brief  Get the angle speed from BMX055. 
 * @param  roll, pitch, yaw
 * @retval None
 */
uint8_t BMX055_get_angleSpeed(float *roll, float *pitch, float *yaw)
{
  *roll = SystemAttitudeRate.Roll;
  *pitch = SystemAttitudeRate.Pitch;
  *yaw = SystemAttitudeRate.Yaw;
  
  return 1;
}
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
