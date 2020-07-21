/**
  ******************************************************************************
  * @file   ：Vison_EngineerAlign.cpp
  * @brief  ：工程车取弹视觉对准的驱动包。用于解析NUC发送的数据包，读入数据，并与NUC
  *           进行交互。
  * @author ：YuyongHu、Mentos_Seetoo
  * @date   ：July,2019
  * @version：1.0.0
  ******************************************************************************
  ==============================================================================
                     ##### How to use this module #####
  ==============================================================================
  (1)创建视觉驱动对象。
  
  (2)在任务中一直使用Check_Link()监测连接状态，收到消息后更新状态设置设备在线，否则
     经过Vison_TMO时间后会判定为离线，直至状态更新。注意更新状态必须要在Check_Link()
     之后，否则会出现一段连接丢失的时间。

  (3)使用DataCapture()解析报文，使用Get_xx()等获取最新读到的信息。
     使用GetCtrlMsg()获得发送给视觉的信息。
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Vision_EngineerAlign.h"

//应答有什么用？？？

/* Functions -----------------------------------------------------------------*/
/**
 * @brief  初始化模式
 * @param  void
 * @retval None
 */ 
VisionAlign_Classdef::VisionAlign_Classdef()
{
  Status    = Connection_Lost;
  Tx_Buffer.targetmode = WATCH_FORWARD;
  Tx_Buffer.Flag = 0x23;
  Tx_Buffer.End = 0x24;
}


/**
 * @brief  设置目标模式
 * @param  para_Mode:要设置的模式
 * @retval None.
 */ 
void VisionAlign_Classdef::SetTargetMode(_VisionMode_Typedef para_Mode)
{
  static _VisionMode_Typedef last_mode = WATCH_FORWARD;
  if(last_mode != para_Mode)
  {
    last_mode = para_Mode;
    Tx_Buffer.targetmode = para_Mode;
    TX_flag = 1;
  }
}


/**
 * @brief  获取在线状态
 * @param  void
 * @retval 当前在线状态
 */ 
//LinkageStatus_Typedef VisionAlign_Classdef::GetStatus()
//{
//   return Status;
//}


/**
 * @brief  向视觉端发送控制信息
 * @param  None.
 * @retval char*:要发送的数据包地址
            NULL:没有数据发送
 */ 
Vision_TxData_Typedef* VisionAlign_Classdef::Get_TxBuff(void)
{
  static int32_t last_send_time = 0;
  //发送完后等待应答
 // if(TX_flag) 
  //{
 //   if(time_stamp - last_send_time >= Vison_ReTransmit)//限制发送频率？？？
 //   {
      last_send_time = time_stamp;
      TX_flag  = 0;
      ACK_flag = 1;
      return &Tx_Buffer;
  //  }
  // else return NULL;
 // }
 // else return NULL;
}

/**
 * @brief  获取视觉数据
 * @param  captureData:视觉数据包的指针，一般为串口的接受缓冲区指针
 * @retval void
 */ 
void VisionAlign_Classdef::DataCapture(Vision_DataPack_Typedef* captureData)
{
  static uint32_t last_time;
  
  //连接检查
  if(time_stamp - last_time > Vison_TMO)
    Status = Connection_Lost;
  else 
    Status = Connection_Established;
  last_time = time_stamp;
  
  Vision_DataPack = *captureData;
  //模式确认,模式不正确时再次发送，等待应答
  if(Vision_DataPack.Mode != Tx_Buffer.targetmode)
    TX_flag = 1;
  else
  {
    ACK_flag = 0;
    TX_flag  = 0;
  }
}

char VisionAlign_Classdef::Is_Detected(void)
{
  if(Vision_DataPack.End == 0x6A) return 1;
  else return 0;
}

/**
* @brief  更新时间戳
* @param  current_time(ms).
* @return None.
*/
void VisionAlign_Classdef::UpdateTimeStamp(uint32_t current_time)
{
  time_stamp = current_time;
}

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
