/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    BMX055_Config.c
  * @author  YDX 2244907035@qq.com
  * @brief   Code for BMX055.
  * @date    2019-11-21
  * @version 1.0
  * @par Change Log:
  * <table>
  * <tr><th>Date        <th>Version  <th>Author  <th>Description
  * <tr><td>2019-11-21  <td> 1.0     <td>YDX     <td>use the codes in mpu6050_config.cpp\n
  *                                                  add macros to config.
  * </table>
  *
  ==============================================================================
                            How to use this driver  
  ==============================================================================
    @note
      -# `BMX055_Init()`进行初始化
         e.g:
         BMX055_Init(GPIOB, GPIO_PIN_10, GPIO_PIN_11, 10, 11, 0.002f);
         
      -# `BMX055_solve_data()`获取数据前先解算
      -# `BMX055_get_angle()`获取角度/`BMX055_get_angleSpeed()`获取角速度
      
    @warning	
      -# 添加预编译宏`USE_FULL_ASSERT`可以启用断言检查。
      -# 需要`SRML`的`drv_i2c`支持。
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
#include "BMX055_Config.h"
#include "Attitude_Calculation.h"
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
IIC_HandleTypeDef BMX055_IIC;
BMX055_IIC_PIN_Typedef BMX055_IIC_PIN;

/* Private type --------------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/	
/**
  * @brief  SCL output level
  * @param  x: output level
  * @retval void    
  */
void IIC_SCL_B(const char x)
{
  (x!=0)?HAL_GPIO_WritePin(BMX055_IIC_PIN.BMX055_IIC_GPIO_PORT,BMX055_IIC_PIN.BMX055_IIC_SCL_PIN,GPIO_PIN_SET)\
	    :HAL_GPIO_WritePin(BMX055_IIC_PIN.BMX055_IIC_GPIO_PORT,BMX055_IIC_PIN.BMX055_IIC_SCL_PIN,GPIO_PIN_RESET);
}

/**
  * @brief  SDA output level
  * @param  x: output level
  * @retval void    
  */
void IIC_SDA_B(const char x)
{
  (x!=0)?HAL_GPIO_WritePin(BMX055_IIC_PIN.BMX055_IIC_GPIO_PORT,BMX055_IIC_PIN.BMX055_IIC_SDA_PIN,GPIO_PIN_SET)\
	    :HAL_GPIO_WritePin(BMX055_IIC_PIN.BMX055_IIC_GPIO_PORT,BMX055_IIC_PIN.BMX055_IIC_SDA_PIN,GPIO_PIN_RESET);
}

/**
  * @brief  SDA Input
  * @param  void	
  * @retval void    
  */
void SDA_IN_B(void)
{
    BMX055_IIC_PIN.BMX055_IIC_GPIO_PORT->MODER&=~(3<<(BMX055_IIC_PIN.BMX055_IIC_SDA_PIN_NUM*2));
    BMX055_IIC_PIN.BMX055_IIC_GPIO_PORT->MODER|= (0<<(BMX055_IIC_PIN.BMX055_IIC_SDA_PIN_NUM*2));	    
}

/**
  * @brief  SDA Output
  * @param  void
  * @retval void   
  */
void SDA_OUT_B(void)
{
    BMX055_IIC_PIN.BMX055_IIC_GPIO_PORT->MODER&=~(3<<(BMX055_IIC_PIN.BMX055_IIC_SDA_PIN_NUM*2));
    BMX055_IIC_PIN.BMX055_IIC_GPIO_PORT->MODER|= (1<<(BMX055_IIC_PIN.BMX055_IIC_SDA_PIN_NUM*2)); 	
}

/**
  * @brief  read SDA
  * @param  void
  * @retval SDA level  
  */
GPIO_PinState READ_SDA_B(void)
{
    GPIO_PinState bitstatus;
	
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(BMX055_IIC_PIN.BMX055_IIC_SDA_PIN));

    if((BMX055_IIC_PIN.BMX055_IIC_GPIO_PORT->IDR & BMX055_IIC_PIN.BMX055_IIC_SDA_PIN) != (uint32_t)GPIO_PIN_RESET)
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
  * @brief  BMX055 initialization
  * @param  gpiox: BMX055 iic gpio port
  * @param  scl_pinx: BMX055 iic scl pin
  * @param  sda_pinx: BMX055 iic sda pin
  * @param  scl_pin_numx: BMX055 iic scl pin number
  * @param  sda_pin_numx: BMX055 iic sda pin number
  * @param  cal_period: attitude calculation period(Unit:second)
  * @retval void    
  */
void BMX055_Init(GPIO_TypeDef* gpiox,uint32_t scl_pinx,uint32_t sda_pinx,uint32_t scl_pin_numx,uint32_t sda_pin_numx,float cal_period)
{
	PERIODS = cal_period;
	PERIODHZ = 1/PERIODS;
	
    /* BMX055_IIC initialization */
    BMX055_IIC_PIN.BMX055_IIC_GPIO_PORT = gpiox;
    BMX055_IIC_PIN.BMX055_IIC_SCL_PIN = scl_pinx;
    BMX055_IIC_PIN.BMX055_IIC_SDA_PIN = sda_pinx;
    BMX055_IIC_PIN.BMX055_IIC_SCL_PIN_NUM = scl_pin_numx;
    BMX055_IIC_PIN.BMX055_IIC_SDA_PIN_NUM = sda_pin_numx;
	
    BMX055_IIC.xIIC_SCL  = IIC_SCL_B;
    BMX055_IIC.xIIC_SDA  = IIC_SDA_B;
    BMX055_IIC.xSDA_IN   = SDA_IN_B;
    BMX055_IIC.xSDA_OUT  = SDA_OUT_B;
    BMX055_IIC.xREAD_SDA = READ_SDA_B;
  
	if(BMX055_IIC.xIIC_SCL == NULL || BMX055_IIC.xIIC_SDA == NULL || BMX055_IIC.xSDA_IN  == NULL || BMX055_IIC.xSDA_OUT == NULL || BMX055_IIC.xREAD_SDA == NULL)
	{
		/* function pointer error */
        assert_param(0);
	}
	else 
	{
		/* BMX055 initialization */
		BMX_Conf(&BMX055_IIC);
	}
}
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
