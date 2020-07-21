/**
  ******************************************************************************
  * @file   ：Vison_EngineerAlign.h
  * @brief  ：Header of Vison_EngineerAlign.cpp
  * @author ：Mentos Seetoo (1356046979@qq.com)
  * @date   ：July,2019
  * @version：1.0.0
  ******************************************************************************
**/

#ifndef __VISION_ENGINEERALIGN__
#define __VISION_ENGINEERALIGN__

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdint.h>

/* Private  ------------------------------------------------------------------*/
#ifdef __cplusplus

#define Vison_TMO             500       //500ms离线计时
#define Vison_ReTransmit      100       //100ms超时重传
//#pragma pack (1)
/* 视觉数据包 */
 struct Vision_DataPack_Typedef
{
  uint8_t Mode;     //当前模式，即GROUND_LEFT
  short Vision_X;
  short Vision_Y;
  short Vision_Yaw;
  uint8_t End;      //如果检测到目标发0x6A，没有检测到则发0x5A;
};

 struct ctrl_Typedef
 {
     unsigned char Flag;//固定为5
     float angle[3];//x ,y ,z角速度
     float speed[3];//x ,y ,yaw线速度
     unsigned char End;//固定为5
 };
 //    float Anx;
 //   float Any;
//     float Anz;
   //   float LF;
  //   float RF;
   //  float RB;
   //  float LB;
 //    float LM;
  //   float RM;
struct Vision_TxData_Typedef
{
  unsigned char Flag;       //固定为6
  uint8_t targetmode;       //仿真中固定为GROUND_LEFT 即0x04
  unsigned char End;        //固定为6
};

enum _DetectStatus_Typedef
{
  UNDETECTED = 0x5A,
  DETECTED   = 0x6A
};

/* 
  视觉模式 (以下方位均相对于屏幕来说）
*/
enum _VisionMode_Typedef
{
  ISLAND_TWO      = 0x01,        //岛上对准(左)边弹药箱(自动取2个)
  ISLAND_THREE    = 0x02,        //岛上对准中间弹药箱(视觉辅助取单个)
  GROUND_RIGHT    = 0x03,        //岛下对准右边弹药箱(自动3)
  GROUND_LEFT     = 0x04,        //岛下对准左边弹药箱(自动取3个)
  GROUND_MID      = 0x05,        //岛下对准中间弹药箱(视觉辅助取单个)
  WATCH_FORWARD   = 0x09,        //屏幕切到模组相机
  WAITCH_AMMO     = 0x07,        //屏幕切到顶层弹药箱相机
  ROTATE_SCREEN   = 0x10,        //屏幕宽高比改变
  RERUN           = 0X11,        //程序重启
  REBOOT          = 0x12,        //电脑重启
  STAND_BY        = 0x13         //识别程序待机，不进行操作，只进行应答
};

/* 识别状态 */

#ifndef __LinkageStatus_DEFINED
#define __LinkageStatus_DEFINED
enum LinkageStatus_Typedef
{
  Connection_Lost = 0U,
  Connection_Established,
};
#endif

/* Exported ------------------------------------------------------------------*/
/* 工程视觉对准驱动 */
class VisionAlign_Classdef
{
private:
    LinkageStatus_Typedef Status;                //连接状态
    uint32_t     time_stamp;                     //时间戳
    uint8_t      TX_flag;                        //等待发送标志
    uint8_t      ACK_flag;                       //等待应答标志
    Vision_TxData_Typedef Tx_Buffer;                              //驱动发送缓冲区

public:

    VisionAlign_Classdef();
    Vision_DataPack_Typedef Vision_DataPack;                 //视觉数据包
    void SetTargetMode(_VisionMode_Typedef para_Mode);       //设置目标模式
    void UpdateTimeStamp(uint32_t current_time);       //更新时间戳
    Vision_TxData_Typedef* Get_TxBuff(void);                                   //获取要发送给视觉的数据包
    void DataCapture(Vision_DataPack_Typedef* captureData);  //获取视觉数据
    char Is_Detected(void);
    bool Is_Connected(void);
};


#endif
#endif
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
