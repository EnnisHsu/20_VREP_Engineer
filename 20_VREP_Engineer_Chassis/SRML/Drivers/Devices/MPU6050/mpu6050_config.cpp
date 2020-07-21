/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    mpu6050_config.cpp
  * @author  YDX 2244907035@qq.com
  * @brief   Code for MPU6050.
  * @date    2019-11-21
  * @version 1.1
  * @par Change Log:
  * <table>
  * <tr><th>Date        <th>Version  <th>Author     <th>Description
  * <tr><td>2018-10-18  <td> 1.0     <td>mannychen  <td>Creator
  * <tr><td>2019-11-21  <td> 1.1     <td>YDX        <td>remove config macro.
  * </table>
  *
  ==============================================================================
                              How to use this driver  
  ==============================================================================
    @note
      -# `MPU6050_Init()`初始化
         e.g:
         MPU6050_Init(GPIOB, GPIO_PIN_6, GPIO_PIN_7, 6, 7);
         
      -# `mpu_dmp_get_data()`获取角度/`MPU_Get_Gyroscope()`获取角速度(原始数据)
         `MPU_Get_Accelerometer()`获取三轴加速度
         e.g:
         if(!MPU_Get_Gyroscope(&MPU6050_IIC, &MPUData.gx, &MPUData.gy, &MPUData.gz))
         {
            MPUData.gx -= MPUData.gxoffset;
            MPUData.gy -= MPUData.gyoffset;
            MPUData.gz -= MPUData.gzoffset;
         }
    @warning	
      -# 添加预编译宏`USE_FULL_ASSERT`可以启用断言检查。
      -# 需要`SRML`的`drv_i2c`支持。
      -# 软件IIC的IO口需要配置成：超高速开漏上拉模式。
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
#include "mpu6050_config.h"
#include "stdio.h"
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
IIC_HandleTypeDef MPU6050_IIC;
MPU6050_IIC_PIN_Typedef MPU6050_IIC_PIN;
MPUData_Typedef MPUData;

/* Private type --------------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  SCL output level
  * @param  x: output level
  * @retval void    
  */
void IIC_SCL_A(const char x)
{
  (x!=0)?HAL_GPIO_WritePin(MPU6050_IIC_PIN.MPU6050_IIC_GPIO_PORT,MPU6050_IIC_PIN.MPU6050_IIC_SCL_PIN,GPIO_PIN_SET)\
	    :HAL_GPIO_WritePin(MPU6050_IIC_PIN.MPU6050_IIC_GPIO_PORT,MPU6050_IIC_PIN.MPU6050_IIC_SCL_PIN,GPIO_PIN_RESET);
}

/**
  * @brief  SDA output level
  * @param  x: output level
  * @retval void    
  */
void IIC_SDA_A(const char x)
{
  (x!=0)?HAL_GPIO_WritePin(MPU6050_IIC_PIN.MPU6050_IIC_GPIO_PORT,MPU6050_IIC_PIN.MPU6050_IIC_SDA_PIN,GPIO_PIN_SET)\
	    :HAL_GPIO_WritePin(MPU6050_IIC_PIN.MPU6050_IIC_GPIO_PORT,MPU6050_IIC_PIN.MPU6050_IIC_SDA_PIN,GPIO_PIN_RESET);
}

/**
  * @brief  SDA Input
  * @param  void	
  * @retval void    
  */
void SDA_IN_A(void)
{
    MPU6050_IIC_PIN.MPU6050_IIC_GPIO_PORT->MODER&=~(3<<(MPU6050_IIC_PIN.MPU6050_IIC_SDA_PIN_NUM*2));
    MPU6050_IIC_PIN.MPU6050_IIC_GPIO_PORT->MODER|= (0<<(MPU6050_IIC_PIN.MPU6050_IIC_SDA_PIN_NUM*2));	    
}

/**
  * @brief  SDA Output
  * @param  void
  * @retval void   
  */
void SDA_OUT_A(void)
{
    MPU6050_IIC_PIN.MPU6050_IIC_GPIO_PORT->MODER&=~(3<<(MPU6050_IIC_PIN.MPU6050_IIC_SDA_PIN_NUM*2));
    MPU6050_IIC_PIN.MPU6050_IIC_GPIO_PORT->MODER|= (1<<(MPU6050_IIC_PIN.MPU6050_IIC_SDA_PIN_NUM*2)); 	
}

/**
  * @brief  read SDA
  * @param  void
  * @retval SDA level  
  */
GPIO_PinState READ_SDA_A(void)
{
    GPIO_PinState bitstatus;
	
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(MPU6050_IIC_PIN.MPU6050_IIC_SDA_PIN));

    if((MPU6050_IIC_PIN.MPU6050_IIC_GPIO_PORT->IDR & MPU6050_IIC_PIN.MPU6050_IIC_SDA_PIN) != (uint32_t)GPIO_PIN_RESET)
    {
        bitstatus = GPIO_PIN_SET;
    }
    else
    {
        bitstatus = GPIO_PIN_RESET;
    }
    return bitstatus;
}

/**
  * @brief  MPU6050 initialization
  * @param  gpiox: MPU6050 iic gpio port
  * @param  scl_pinx: MPU6050 iic scl pin
  * @param  sda_pinx: MPU6050 iic sda pin
  * @param  scl_pin_numx: MPU6050 iic scl pin number
  * @param  sda_pin_numx: MPU6050 iic sda pin number
  * @retval void    
  */
void MPU6050_Init(GPIO_TypeDef* gpiox,uint32_t scl_pinx,uint32_t sda_pinx,uint32_t scl_pin_numx,uint32_t sda_pin_numx)
{
    /* MPU6050_IIC initialization */
	MPU6050_IIC_PIN.MPU6050_IIC_GPIO_PORT = gpiox;
	MPU6050_IIC_PIN.MPU6050_IIC_SCL_PIN = scl_pinx;
	MPU6050_IIC_PIN.MPU6050_IIC_SDA_PIN = sda_pinx;
    MPU6050_IIC_PIN.MPU6050_IIC_SCL_PIN_NUM = scl_pin_numx;
	MPU6050_IIC_PIN.MPU6050_IIC_SDA_PIN_NUM = sda_pin_numx;
	
    MPU6050_IIC.xIIC_SCL = IIC_SCL_A;
    MPU6050_IIC.xIIC_SDA = IIC_SDA_A;
    MPU6050_IIC.xSDA_IN =  SDA_IN_A;
    MPU6050_IIC.xSDA_OUT = SDA_OUT_A;
    MPU6050_IIC.xREAD_SDA = READ_SDA_A;
  
	if(MPU6050_IIC.xIIC_SCL == NULL || MPU6050_IIC.xIIC_SDA == NULL || MPU6050_IIC.xSDA_IN  == NULL || MPU6050_IIC.xSDA_OUT == NULL || MPU6050_IIC.xREAD_SDA == NULL)
	{
		/* function pointer error */
        assert_param(0);
	}
	else 
	{
		/* MPU6050 initialization */
		MPU_Init(&MPU6050_IIC);
	    if(mpu_dmp_init()!=0)
	    {
		    HAL_Delay(100);
		    __set_FAULTMASK(1);//reset
		    NVIC_SystemReset();
	    }
	    MPU_Get_Gyroscope_Init(&MPU6050_IIC, &MPUData.gxoffset, &MPUData.gyoffset, &MPUData.gzoffset);
	}
}

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
