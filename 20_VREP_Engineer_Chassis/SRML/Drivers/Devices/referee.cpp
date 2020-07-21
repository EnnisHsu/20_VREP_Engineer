/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file   : referee.cpp
  * @author : charlie 602894526@qq.com
  * @brief  : Code for communicating with Referee system of Robomaster 2019.
  * @date   : 2019-03-01
  * @par Change Log：
  *  Date           Author   Version    Notes
  *  2019-03-01     charlie   2.0.0     Creator
	*  2019-12-28     kainan		3.0.0     增加绘画类
  ==============================================================================
                          How to use this driver  
  ==============================================================================
	Set_refereeUart()设置referee所用串口
	要等待一段时间(等串口、裁判系统稳定)，再发送clean、数据、UI等 
	特别注意要用最新的裁判系统客户端，旧版有点问题
	
	裁判系统数据接收与外部读取
  1. 用串口底层接收裁判系统数据，然后把数据丢进消息队列里面。dataQueueSendByU4()
  2. 单独开一个任务,本工程为refereeUnpack_task(),在任务里面调用unPackDataFromRF()
  3. 在RefereeHandle()加入需要接收的包的cmd_id 具体详见referee.h->RefereeSystemID_t
	4.外部读取直接读取相应的数据结构体即可
	
  机器人车间通信
	0. 车间通信暂时只能发送1个data，裁判系统是允许113个字节的数据的，后期有必要再改进
  1. 在CV_ToOtherRobot()里面设置target_id与data数据,target_id可以通过robot_client_ID查询接收方机器人ID
  2. 在发送端机器人工程里面，调用CV_ToOtherRobot()，添加需要发送的data数据
  3. 外部重写RobotInteractiveHandle()，内部判断接收ID是否为本地ID，是则进行数据处理

  操作手界面数据
  1. 在CV_ToServer()里面设置data1~3与小灯的颜色
  2. 注意一定要让机器人的裁判系统连入服务器，不然无法使用

	操作手界面UI
	1.Set_DrawingLayer()
	2.对应的drawin()，clean()
	3.组合图形:标尺UI_ruler()
	4.注意：发数据给裁判系统。务必注意等待上电稳定之后才发送，否则会丢包 \n
					注意帧间隔时间>110ms； \n
					每个图形的name必须不同，建议name[3]不同即可 \n
	
  目前参考的是裁判系统串口协议附录V2.0-2019-07-23
  如有问题，请参考《RM2019裁判系统用户接口协议附录》
  2019年度裁判系统资料大全：
  https://bbs.robomaster.com/thread-7099-1-1.html
  ******************************************************************************
  * @attention:
  * 
  * if you had modified this file, please make sure your code does not have many 
  * bugs, update the version NO., write dowm your name and the date, the most
  * important is make sure the users will have clear and definite understanding 
  * through your new brief.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "referee.h"

/* Private define ------------------------------------------------------------*/
/* Private function declarations --------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static const unsigned char CRC8_TAB[256] = {
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3,
    0xfd, 0x1f, 0x41, 0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01,
    0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe,
    0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62, 0xbe, 0xe0, 0x02, 0x5c,
    0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff, 0x46,
    0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb,
    0x59, 0x07, 0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5,
    0xfb, 0x78, 0x26, 0xc4, 0x9a, 0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
    0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24, 0xf8, 0xa6, 0x44, 0x1a, 0x99,
    0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 0x8c, 0xd2,
    0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93,
    0xcd, 0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31,
    0xb2, 0xec, 0x0e, 0x50, 0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d,
    0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d,
    0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73, 0xca, 0x94, 0x76,
    0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4,
    0xaa, 0x48, 0x16, 0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75,
    0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8, 0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9,
    0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};

static const uint16_t wCRC_Table[256] = {
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e,
    0xf8f7, 0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64,
    0xf9ff, 0xe876, 0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd, 0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e,
    0xfae7, 0xc87c, 0xd9f5, 0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 0xbdcb, 0xac42, 0x9ed9, 0x8f50,
    0xfbef, 0xea66, 0xd8fd, 0xc974, 0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb, 0xce4c, 0xdfc5, 0xed5e,
    0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a, 0xdecd, 0xcf44,
    0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72, 0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 0xef4e,
    0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70, 0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e,
    0xf0b7, 0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff, 0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324,
    0xf1bf, 0xe036, 0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e,
    0xf2a7, 0xc03c, 0xd1b5, 0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 0xb58b, 0xa402, 0x9699, 0x8710,
    0xf3af, 0xe226, 0xd0bd, 0xc134, 0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c, 0xc60c, 0xd785, 0xe51e,
    0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb, 0xd68d, 0xc704,
    0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 0xe70e,
    0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1, 0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1,
    0x0f78
};
/* Private type --------------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
 * @brief 
 * @note use in SendDrawData() and character_drawing()
 * @param 
 * @retval 
 */
template<typename Type>
Type _referee_Constrain(Type input,Type min,Type max){
  if (input <= min)
    return min;
  else if(input >= max)
    return max;
  else return input;
}

void referee_Classdef::Set_refereeUart(UART_HandleTypeDef *_huart)
{
	refereeUart = _huart;
}

/**
  * @brief   Referee data check
  * @param   **p: Pointer to checked data
  * @retval  0:wrong 1:ture
  */
uint8_t referee_Classdef::DataCheck(uint8_t **p)
{
    static uint16_t Length;
    static uint8_t ShortData[256]; //碎片缓冲区
    if((*p)[0] < 44) {
        uint8_t cnt;
        if(((FrameHeader *)&((*p)[1]))->SOF == 0xA5) {
            Length = 0;
            if(((FrameHeader *)&((*p)[1]))->DataLength == (*p)[0] - 9)
                return 1;
        }
        for(cnt = 0; cnt < (*p)[0]; cnt++)
            ShortData[Length + cnt +1] = (*p)[cnt+1];
        Length += (*p)[0];
        if(Length == 44||Length == 56||Length == 69||Length >= 88) {
            ShortData[0] = Length;
            if(Length == 44||Length == 56||Length == 69)
                *p = &ShortData[0];
            Length = 0;
            return 1;
        } else
            return 0;
    } else
        return 1;
}

/**
  * @brief   CRC8 data check.
  * @param   *pchMessage:Data to be processed
              dwLength:Length of check data
              ucCRC8:Data after processing
  * @retval  	Gets the CRC8 checksum
  */
unsigned char referee_Classdef::Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8)
{
    unsigned char ucIndex;
    while (dwLength--) {
        ucIndex = ucCRC8^(*pchMessage++);
        ucCRC8 = CRC8_TAB[ucIndex];
    }
    return(ucCRC8);
}
/**
  * @brief   CRC16 data check.
  * @param   *pchMessage:Data to be processed
             dwLength:Length of check data
             ucCRC8:Data after processing
  * @retval  Gets the CRC16 checksum
  */
uint16_t referee_Classdef::Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC)
{
    uint8_t chData;
    if (pchMessage == NULL) {
        return 0xFFFF;
    }
    while(dwLength--) {
        chData = *pchMessage++;
        (wCRC) = ((uint16_t)(wCRC) >> 8) ^ wCRC_Table[((uint16_t)(wCRC) ^ (uint16_t)(chData)) & 0x00ff];
    }
    return wCRC;
}


void referee_Classdef::unPackDataFromRF(uint8_t *data_buf, uint32_t length)
{
		static uint8_t temp[128];
		static uint8_t RFdataBuff[256];

		static int32_t index,buff_read_index;
		static short CRC16_Function,CRC16_Referee;
		static uint8_t byte;
		static int32_t read_len;
		static uint16_t data_len;
		static uint8_t unpack_step;
		static uint8_t protocol_packet[PROTOCAL_FRAME_MAX_SIZE];
    /*初始化读取状态*/
    buff_read_index = 0;
    memcpy(RFdataBuff,data_buf,length);
    /*从头开始读取 */
    read_len=length;

    while (read_len--) {
        byte = RFdataBuff[buff_read_index++];
        switch(unpack_step) {
        case STEP_HEADER_SOF: {
            if(byte == START_ID) {
                unpack_step = STEP_LENGTH_LOW;
                protocol_packet[index++] = byte;
            } else {
                index = 0;
            }
        }
        break;

        case STEP_LENGTH_LOW: {
            data_len = byte;
            protocol_packet[index++] = byte;
            unpack_step = STEP_LENGTH_HIGH;
        }
        break;

        case STEP_LENGTH_HIGH: {
            data_len |= (byte << 8);
            protocol_packet[index++] = byte;
            if(data_len < (PROTOCAL_FRAME_MAX_SIZE - HEADER_LEN - CRC_ALL_LEN)) {
                unpack_step = STEP_FRAME_SEQ;
            } else {
                unpack_step = STEP_HEADER_SOF;
                index = 0;
            }
        }
        break;

        case STEP_FRAME_SEQ: {
            protocol_packet[index++] = byte;
            unpack_step = STEP_HEADER_CRC8;
        }
        break;

        case STEP_HEADER_CRC8: {
            protocol_packet[index++] = byte;

            if (index == HEADER_LEN+1) {
                if ( Get_CRC8_Check_Sum(protocol_packet, HEADER_LEN,0xff)== protocol_packet[HEADER_LEN]) {
                    unpack_step = STEP_DATA_CRC16;
                } else {
                    unpack_step = STEP_HEADER_SOF;
                    index = 0;
                }
            }
        }
        break;

        case STEP_DATA_CRC16: {
            if (index < (HEADER_LEN + CMD_LEN + data_len + CRC_ALL_LEN)) {
                protocol_packet[index++] = byte;
            }
            if (index >= (HEADER_LEN + CMD_LEN + data_len + CRC_ALL_LEN)) {

                CRC16_Function=Get_CRC16_Check_Sum(protocol_packet, HEADER_LEN + CMD_LEN + data_len +CRC_8_LEN,0xffff);
                CRC16_Referee=* (__packed short *)(&protocol_packet[index-2]);
                if ( CRC16_Function==CRC16_Referee) {
                    RefereeHandle(protocol_packet);
                }
                unpack_step = STEP_HEADER_SOF;
                index = 0;
            }
        }
        break;

        default: {
            unpack_step = STEP_HEADER_SOF;
            index = 0;
        }
        break;
        }
    }
}

/**
  * @brief  Receive and handle referee system data
  * @param  void
  * @retval void
  */
void referee_Classdef::RefereeHandle(uint8_t *data_buf)
{
    static uint16_t shoot_count = 0;

    switch(((FrameHeader *)data_buf)->CmdID) {
    case GameRobotState_ID: {
        GameRobotState = *(ext_game_robot_status_t*)(&data_buf[7]);
				Calc_Robot_ID(GameRobotState.robot_id);
    }
    break;
    case PowerHeatData_ID:{
        PowerHeatData = *(ext_power_heat_data_t*)(&data_buf[7]);
    }
    break;
    case RobotHurt_ID: {    //0x0206
        RobotHurt = *(ext_robot_hurt_t*)(&data_buf[7]);
    }
    break;
    case ShootData_ID: {    //0x0207
        ShootData = *(ext_shoot_data_t*)(&data_buf[7]);
        shoot_count++;
    }
    break;
    case StudentInteractiveHeaderData_ID: {
        RobotInteractiveData = *(robot_interactive_data_t*)(&data_buf[7]);
    }
    break;
    default:
        break;
    }
}

/**
  * @brief  Calculate robot ID 
  * @param  local robot id
  * @retval 
  */
void referee_Classdef::Calc_Robot_ID(uint8_t local_id)
{
	if(local_id !=0 )	/* referee connection successful */
	{
		if(local_id < 10)	/* red */
		{
			robot_client_ID.hero = 1;
			robot_client_ID.engineer = 2;
			robot_client_ID.infantry_3 = 3;
			robot_client_ID.infantry_4 = 4;
			robot_client_ID.infantry_5 = 5;
			robot_client_ID.aerial = 6;
			robot_client_ID.sentry = 7;
			robot_client_ID.local = local_id;
			robot_client_ID.client = 0x0100 + local_id;
		}
		else	/* blue */
		{
			robot_client_ID.hero = 11;
			robot_client_ID.engineer = 12;
			robot_client_ID.infantry_3 = 13;
			robot_client_ID.infantry_4 = 14;
			robot_client_ID.infantry_5 = 15;
			robot_client_ID.aerial = 16;
			robot_client_ID.sentry = 17;
			robot_client_ID.local = local_id;
			robot_client_ID.client = 0x0110 + local_id;		
		}
	}
}

/**
  * @brief  Transfer user system data to the server through huart
	* @note  	Referee.CV_ToServer(, , , , LIGHT_1|LIGHT_2|LIGHT_3, GREEN_LIGHT);
  * @param  data1,data2,data3,light&color:data to send, light and color use Exported macros
  * @retval void
  */
void referee_Classdef::CV_ToServer(float data1,float data2,float data3, uint8_t light, uint8_t color)
{
		static uint8_t seq_cnt = 0;
		static uint8_t temp[30];

    uint8_t *pMsg=temp;
    ((FrameHeader*)pMsg)->SOF = START_ID;
    ((FrameHeader*)pMsg)->DataLength = 19;
    ((FrameHeader*)pMsg)->Seq = seq_cnt;
    ((FrameHeader*)pMsg)->CRC8 = Get_CRC8_Check_Sum(temp,4,0xff);
    ((FrameHeader*)pMsg)->CmdID = StudentInteractiveHeaderData_ID;
    ((client_custom_data_t*)(pMsg+7))->data_cmd_id = ServerData_ID;
    ((client_custom_data_t*)(pMsg+7))->sender_ID = robot_client_ID.local;
    ((client_custom_data_t*)(pMsg+7))->receiver_ID = robot_client_ID.client;
    ((client_custom_data_t*)(pMsg+7))->data1=data1;
    ((client_custom_data_t*)(pMsg+7))->data2=data2;
    ((client_custom_data_t*)(pMsg+7))->data3=data3;
    ((client_custom_data_t*)(pMsg+7))->masks=(light&color);

    *(__packed short *)(&temp[26]) = Get_CRC16_Check_Sum(temp,26,0xffff);//默认是小端模式

    HAL_UART_Transmit_DMA(refereeUart,temp,28);
    seq_cnt++;
    if(seq_cnt>254)seq_cnt = 0;else{}
}

/**
  * @brief  Transfer user system data to the server through huart
	* @note  	Referee.CV_ToOtherRobot(, Referee.robot_client_ID.hero, 123);
  * @param  data1,data2,data3,data4:data to send
  * @retval void
  */
void referee_Classdef::CV_ToOtherRobot(uint8_t target_id, uint8_t data1)
{
		static uint8_t seq_cnt = 0;
		static uint8_t temp[20];

    uint8_t *pMsg = temp;
    ((FrameHeader*)pMsg)->SOF = START_ID;
    ((FrameHeader*)pMsg)->DataLength = 6 + 1;
    ((FrameHeader*)pMsg)->Seq = seq_cnt;
    ((FrameHeader*)pMsg)->CRC8 = Get_CRC8_Check_Sum(temp,4,0xff);
    ((FrameHeader*)pMsg)->CmdID = StudentInteractiveHeaderData_ID;
    ((robot_interactive_data_t*)(pMsg+7))->data_cmd_id = RobotComData_ID;
		
		((robot_interactive_data_t*)(pMsg+7))->sender_ID = robot_client_ID.local;
		((robot_interactive_data_t*)(pMsg+7))->sender_ID = target_id;
	
    ((robot_interactive_data_t*)(pMsg+7))->data = data1;

    *(__packed short *)(&temp[14]) = Get_CRC16_Check_Sum(temp,14,0xffff);//默认是小端模式
    HAL_UART_Transmit_DMA(refereeUart,temp,16);
		seq_cnt++;
		if(seq_cnt>254)seq_cnt = 0;
}

uint8_t referee_Classdef::RobotInteractiveHandle(robot_interactive_data_t* RobotInteractiveData_t)
{
    if(GameRobotState.robot_id == RobotInteractiveData_t->receiver_ID && GameRobotState.robot_id != 0) {
        if(RobotInteractiveData_t->data_cmd_id == RobotComData_ID) {
            return RobotInteractiveData_t->data;
        }
        else return 0;
    } else return 0;
}

void referee_Classdef::SendDrawData()
{
	static unsigned char temp[75];	
	static unsigned char seq_cnt=0;	

	drawing.SOF = START_ID;
	drawing.DataLength = 61;
	drawing.Seq = seq_cnt;
	drawing.CRC8 = 0;//?
	drawing.CmdID = StudentInteractiveHeaderData_ID;
	drawing.data_cmd_id = DrawingData_ID;
	drawing.send_ID = robot_client_ID.local;
	drawing.receiver_ID = robot_client_ID.client;
		
	seq_cnt++;
	if(seq_cnt>254) seq_cnt=0;
	
	drawing.start_x = _referee_Constrain(drawing.start_x, (uint16_t)0, (uint16_t)1920);
	drawing.start_y = _referee_Constrain(drawing.start_y, (uint16_t)0, (uint16_t)1080);
	drawing.end_x = _referee_Constrain(drawing.end_x, (uint16_t)0, (uint16_t)1920);
	drawing.end_y = _referee_Constrain(drawing.end_y, (uint16_t)0, (uint16_t)1080);
	
	memcpy((void*)temp,&drawing,68);
	temp[4]=Get_CRC8_Check_Sum(temp,4,0xff);
	* (__packed short *)(&temp[68]) = Get_CRC16_Check_Sum(temp,68,0xffff);
	HAL_UART_Transmit_DMA(refereeUart,temp,70);
}

/**
 * @brief Set the painting layer
 * @note Referee.line_drawing(ADD_PICTURE, 400,500,1000,900,GREEN, test);
				 A large digital layer covers a small digital layer
 * @param 
 * @retval 
 */
void referee_Classdef::Set_DrawingLayer(uint8_t _layer)
{
	drawing.layer = _layer;
}

/**
 * @brief Draw a straight line at the UI interface
 * @note Referee.line_drawing(ADD_PICTURE, 400,500,1000,900,GREEN, test);
 * @param 
 * @retval 
 */
void referee_Classdef::line_drawing(drawOperate_e _operate_type, uint16_t startx,uint16_t starty,uint16_t endx,uint16_t endy,colorType_e vcolor, uint8_t name[])
{
	memcpy(drawing.graphic_name,name,5);
	drawing.operate_tpye = _operate_type;
	drawing.graphic_tpye = 1;
	drawing.width=5;
	drawing.color=vcolor;
	drawing.start_x=startx;
	drawing.start_y=starty;
	drawing.end_x=endx;
	drawing.end_y=endy;
	SendDrawData();	
}

/**
 * @brief 
 * @note Referee.rectangle_drawing(ADD_PICTURE, 700,300,200,900,GREEN, test);
 * @param 
 * @retval 
 */
void referee_Classdef::rectangle_drawing(drawOperate_e _operate_type, uint16_t startx,uint16_t starty,uint16_t length_,uint16_t width_,colorType_e vcolor, uint8_t name[])
{
	memcpy(drawing.graphic_name, name, 5);	
	drawing.operate_tpye = _operate_type;
	drawing.graphic_tpye = 2;
	drawing.width=5;
	drawing.color=vcolor;
	drawing.start_x=startx;
	drawing.start_y=starty;
	drawing.end_x=startx+length_;
	drawing.end_y=starty+width_;
	SendDrawData();
}

/**
 * @brief 
 * @note Referee.rectangle_drawing(ADD_PICTURE, 700,300,200,900,GREEN, test);
 * @param 
 * @retval 
 */
void referee_Classdef::circle_drawing(drawOperate_e _operate_type, uint16_t centrex,uint16_t centrey,uint16_t r,colorType_e vcolor, uint8_t name[])
{
	memcpy(drawing.graphic_name, name, 5);		
	drawing.operate_tpye = _operate_type;
	drawing.graphic_tpye = 3;
	drawing.width=5;
	drawing.color=vcolor;
	drawing.start_x=centrex;
	drawing.start_y=centrey;
	drawing.radius=r;
	SendDrawData();
}
/**
 * @brief 
 * @note Referee.oval_drawing(ADD_PICTURE, 800,500,200,500,GREEN,test);	 
 * @param 
 * @retval 
 */
void referee_Classdef::oval_drawing(drawOperate_e _operate_type, uint16_t centrex,uint16_t centrey,uint16_t minor_semi_axis,uint16_t major_semi_axis,colorType_e vcolor, uint8_t name[])
{
	memcpy(drawing.graphic_name, name, 5);	
	drawing.operate_tpye = _operate_type;	
	drawing.graphic_tpye=4;
	drawing.width=5;
	drawing.color=vcolor;
	drawing.start_x=centrex;
	drawing.start_y=centrey;
	drawing.end_x=major_semi_axis;
	drawing.end_y=minor_semi_axis;
	SendDrawData();
	
}

/**
 * @brief 
 * @note Referee.arc_drawing(ADD_PICTURE, 800,500,300,500,30,150,PURPLE, test);
 * @param 
 * @retval 
 */
void referee_Classdef::arc_drawing(drawOperate_e _operate_type, uint16_t centrex,uint16_t centrey,uint16_t endx,uint16_t endy,int16_t start_angle_,int16_t end_angle_,colorType_e vcolor, uint8_t name[])
{
	memcpy(drawing.graphic_name, name, 5);	
	drawing.operate_tpye = _operate_type;	
	drawing.graphic_tpye=5;
	drawing.width=5;
	drawing.color=vcolor;
	drawing.start_x=centrex;
	drawing.start_y=centrey;
	drawing.end_x=endx;
	drawing.end_y=endy;
	drawing.start_angle=start_angle_;
	drawing.end_angle=end_angle_;
	SendDrawData();

}

/**
 * @brief 
 * @note Referee.character_drawing(ADD_PICTURE, 800,500,30,3, test, BLUE, test);
				 Character length not exceeding 30		 
 * @param 
 * @retval 
 */
void referee_Classdef::character_drawing(drawOperate_e _operate_type, uint16_t startx,uint16_t starty,uint16_t size, uint8_t length,uint8_t character[], colorType_e vcolor, uint8_t name[])
{
	length = _referee_Constrain(length, (uint8_t)0, (uint8_t)30);
	
	memcpy(drawing.graphic_name, name, 5);		
	drawing.operate_tpye = _operate_type;	
	drawing.graphic_tpye=6;
	drawing.width=5;
	drawing.color=vcolor;
	drawing.start_x=startx;
	drawing.start_y=starty;
	drawing.radius=size;
	drawing.text_lenght=length;
	memcpy(drawing.text,character,length);
	SendDrawData();

}

/**
 * @brief 
 * @note Referee.clean_one_picture(2, test);
 * @param 
 * @retval 
 */
void referee_Classdef::clean_one_picture(uint8_t vlayer,uint8_t name[])
{
	memcpy(drawing.graphic_name, name, 5);		
	drawing.layer = vlayer ;
	drawing.operate_tpye=CLEAR_ONE_PICTURE;
	SendDrawData();
}
/**
 * @brief 
 * @note Referee.clean_layer(2);
 * @param 
 * @retval 
 */
void referee_Classdef::clean_layer(uint8_t _layer)
{
	drawing.operate_tpye=4;
	drawing.layer=CLEAR_ONE_LAYER;
	SendDrawData();
}
/**
 * @brief 
 * @note Referee.clean_all();
 * @param 
 * @retval 
 */
void referee_Classdef::clean_all()
{
	drawing.operate_tpye=CLEAR_ALL;
	SendDrawData();
}
/**
 * @brief draw_ruler
 * @note 	#define CIRCLE 24  准心圆半径 
* @param _sys_time, sacle_num多少条刻度线(<9),ruler_tag第几条标尺, startpoint(标尺左上角起点), step(间距),scale_long(长刻度线的长度),scale_short
 * @retval done:1, if no done:0
 */
uint8_t referee_Classdef::UI_ruler(uint32_t _sys_time, uint8_t ruler_tag, uint8_t sacle_num,uint16_t start_x, uint16_t start_y, uint16_t step, uint16_t scale_long, uint16_t scale_short, colorType_e _color)
{	
	static uint8_t scale_cnt = 0;
	static uint8_t draw_cnt = 0;
	static uint32_t last_sys_time = 0;

	uint8_t name[6]="ruler";
	uint16_t axis_temp;
	
	if(draw_cnt<5) /* 画5次 */
	{
		if(_sys_time - 200 >= last_sys_time)	/* 单位: ms ，频率不能大于10Hz*/
		{
			axis_temp = start_y - scale_cnt*step;
			name[3] = ruler_tag;
			name[4] = scale_cnt;
			
			if(scale_cnt == 0)
			{
				line_drawing(ADD_PICTURE, start_x,start_y,start_x,start_y-(sacle_num+1)*step,_color, name); //竖				
			}
			else
			{
				if(scale_cnt%2 == 1)
					line_drawing(ADD_PICTURE, start_x,axis_temp,start_x+scale_long,axis_temp,_color, name);   //长
				else
					line_drawing(ADD_PICTURE, start_x,axis_temp,start_x+scale_short,axis_temp,_color, name);  //短
			}
			scale_cnt++;
			last_sys_time = _sys_time;						
			if(scale_cnt>sacle_num)
			{
				draw_cnt++;
				scale_cnt = 0;
			}
		}
		return 0;
	}
	else
	{
		scale_cnt = 0;
		draw_cnt = 0;
		return 1;
	}
}


/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
