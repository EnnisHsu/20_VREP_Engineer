/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file   : referee.cpp
  * @author : charlie 602894526@qq.com
  * @brief  : Code for communicating with Referee system of Robomaster 2020.
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have many 
  * bugs, update the version NO., write dowm your name and the date, the most
  * important is make sure the users will have clear and definite understanding 
  * through your new brief.
  ******************************************************************************
  */
#ifndef __REFEREE_H__
#define __REFEREE_H__

/* Includes ------------------------------------------------------------------*/
#include  <stdint.h>
#include  <string.h>
#include "../Components/drv_uart.h"
#ifdef __cplusplus
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define LIGHT_1  (0x01)
#define LIGHT_2  (0x01<<1)
#define LIGHT_3  (0x01<<2)
#define LIGHT_4  (0x01<<3)
#define LIGHT_5  (0x01<<4)
#define LIGHT_6  (0x01<<5)

#define RED_LIGHT 0x00
#define GREEN_LIGHT 0xff

/* Private type --------------------------------------------------------------*/
enum RF_status_e
{
  RF_OFFLINE = 0U,
	RF_ONLINE
};

/**
	@brief robot and client ID
*/
typedef	struct { 
	uint8_t hero;
	uint8_t engineer;
	uint8_t infantry_3;
	uint8_t infantry_4;
	uint8_t infantry_5;
	uint8_t aerial;
	uint8_t sentry;
	uint8_t local;
	uint16_t client;
} RC_ID;

/*裁判系统数据结构*/
typedef __packed struct { 
  uint8_t SOF;
  uint16_t DataLength;
  uint8_t Seq;
  uint8_t CRC8;
  uint16_t CmdID;
} FrameHeader;

/**
   @brief 裁判系统主控数据接收部分 
*/
typedef enum {
  GameState_ID                    = 0x0001,
  GameResult_ID                   = 0x0002,
  GameRobotHP_ID                  = 0x0003,
  EventData_ID                    = 0x0101,
  SupplyProjectileAction_ID       = 0x0102,
  SupplyProjectileBooking_ID      = 0x0103,
  RefereeWarning_ID               = 0x0104,
  GameRobotState_ID               = 0x0201,
  PowerHeatData_ID                = 0x0202,
  GameRobotPos_ID                 = 0x0203,
  BuffMusk_ID                     = 0x0204,
  AerialRobotEnergy_ID            = 0x0205,
  RobotHurt_ID                    = 0x0206,
  ShootData_ID                    = 0x0207,
  BulletRemaining_ID              = 0x0208,
  StudentInteractiveHeaderData_ID	= 0x0301, /* 车间通信和客户端都是0x0301 */
	DrawingData_ID									= 0x0100,
	ServerData_ID										= 0xD180,
	RobotComData_ID									= 0x0233	/* 车间交互，队伍自定义 */
} RefereeSystemID_t;

/**
  @brief  比赛状态数据：0x0001  1Hz
*/
typedef __packed struct {
  uint8_t game_type : 4;
  uint8_t game_progress : 4;
  uint16_t stage_remain_time;
} ext_game_status_t;

/**
   @brief 比赛结果数据：0x0002	比赛结束后发送
*/
typedef __packed struct {
  uint8_t winner;
} ext_game_result_t;

/**
   @brief 机器人血量数据：0x0003	1Hz
*/
typedef __packed struct { 
  uint16_t red_1_robot_HP;
  uint16_t red_2_robot_HP;
  uint16_t red_3_robot_HP;
  uint16_t red_4_robot_HP;
  uint16_t red_5_robot_HP;
  uint16_t red_7_robot_HP;
  uint16_t red_base_HP;
  uint16_t blue_1_robot_HP;
  uint16_t blue_2_robot_HP;
  uint16_t blue_3_robot_HP;
  uint16_t blue_4_robot_HP;
  uint16_t blue_5_robot_HP;
  uint16_t blue_7_robot_HP;
  uint16_t blue_base_HP;
} ext_game_robot_HP_t;

/**
   @brief 场地事件数据：0x0101	事件改变发送
*/
typedef __packed struct {
  uint32_t event_type;
} ext_event_data_t;

/**
   @brief 补给站动作标识：0x0102	动作改变后发送
*/
typedef __packed struct {
  uint8_t supply_projectile_id;
  uint8_t supply_robot_id;
  uint8_t supply_projectile_step;
  uint8_t supply_projectile_num;  
} ext_supply_projectile_action_t;

/**
   @brief 补给站预约子弹：0x0103	上限10Hz
*/
typedef __packed struct {   
  uint8_t supply_robot_id; 
  uint8_t supply_projectile_id;
  uint8_t supply_num;
} ext_supply_projectile_booking_t;

/**
   @brief 裁判警告信息：0x0104	警告发生后发送
*/
typedef __packed struct {
    uint8_t level;
    uint8_t foul_robot_id;
} ext_referee_warning_t;

/**
   @brief 比赛机器人状态：0x0201	10Hz
*/
typedef __packed struct { 
  uint8_t robot_id;
  uint8_t robot_level;
  uint16_t remain_HP;
  uint16_t max_HP;
  uint16_t shooter_heat0_cooling_rate;
  uint16_t shooter_heat0_cooling_limit;
  uint16_t shooter_heat1_cooling_rate;
  uint16_t shooter_heat1_cooling_limit;
  uint8_t mains_power_gimbal_output : 1;
  uint8_t mains_power_chassis_output : 1;
  uint8_t mains_power_shooter_output : 1;
} ext_game_robot_status_t;   

/**
   @brief 实时功率热量数据：0x0202	50Hz
*/
typedef __packed struct {
  uint16_t chassis_volt;
  uint16_t chassis_current;
  float chassis_power;
  uint16_t chassis_power_buffer;
  uint16_t shooter_heat0;
  uint16_t shooter_heat1;
} ext_power_heat_data_t;

/**
   @brief 机器人位置：0x0203	10Hz
*/
typedef __packed struct { 
  float x;
  float y;
  float z;
  float yaw;
} ext_game_robot_pos_t;

/**
   @brief 机器人增益：0x0204	状态改变后发送
*/
typedef __packed struct {
  uint8_t power_rune_buff;
} ext_buff_t;

/**
   @brief 空中机器人能量状态：0x0205	10Hz
*/
typedef __packed struct {
  uint8_t energy_point;
  uint8_t attack_time;
} aerial_robot_energy_t;

/**
   @brief 伤害状态：0x0206	收到伤害后发送
*/
typedef __packed struct {
  uint8_t armor_id : 4;
  uint8_t hurt_type : 4;
} ext_robot_hurt_t;

/**
   @brief 实时射击信息：0x0207	射击后发送
*/
typedef __packed struct {
  uint8_t bullet_type;
  uint8_t bullet_freq;
  float bullet_speed;
} ext_shoot_data_t;

/**
   @brief 子弹剩余发射数：0x0208	1Hz
*/
typedef __packed struct {
  uint16_t bullet_remaining_num;
} ext_bullet_remaining_t;

/* 裁判系统客户端交互部分 */
/**
   @brief 交互数据接收信息：0x0301	10Hz
*/
typedef __packed struct {
  uint16_t data_cmd_id;
  uint16_t sender_ID;     
  uint16_t receiver_ID;
} ext_student_interactive_header_data_t;

/**
   @brief 客户端自定义数据：0x0301	内容ID：0xD180 10Hz
*/
typedef __packed struct {
	uint16_t data_cmd_id;
	uint16_t sender_ID;
	uint16_t receiver_ID;
	float data1;
    float data2;
    float data3;
    uint8_t masks;
} client_custom_data_t;

/**
   @brief 学生机器人间通信：0x0301	内容ID：0x0201~0x02FF	10Hz
*/
typedef __packed struct {
	uint16_t data_cmd_id;
	uint16_t sender_ID;
	uint16_t receiver_ID;
	uint8_t data;           //!<长度需要小于113个字节
} robot_interactive_data_t;

/* 解包用的协议 */
#define START_ID	0XA5
#define PROTOCAL_FRAME_MAX_SIZE	25
#define HEADER_LEN 	4
#define CRC_ALL_LEN	3
#define CRC_8_LEN	1
#define CMD_LEN	2

typedef enum {
    STEP_HEADER_SOF=0,
    STEP_LENGTH_LOW,
    STEP_LENGTH_HIGH,
    STEP_FRAME_SEQ,
    STEP_HEADER_CRC8,
    STEP_DATA_CRC16
} unPackStep_e;

/* Exported ------------------------------------------------------------------*/
typedef __packed struct 
{
	 uint8_t _SOF;
	 uint8_t seq; //包序列，用于图形命名
	 uint16_t start_x;
	 uint16_t start_y;
	 uint16_t end_x;
	 uint16_t end_y;
	 uint8_t _EOF;             
} uart_pack_s;

typedef struct
{
	uint8_t  SOF;
	uint16_t DataLength;
	uint8_t  Seq;
	uint8_t  CRC8;
	uint16_t CmdID;
	uint16_t data_cmd_id;
	uint16_t send_ID;
	uint16_t receiver_ID;
	uint8_t operate_tpye;
	uint8_t graphic_tpye;
	uint8_t graphic_name[5];
	uint8_t layer;
	uint8_t color;
	uint8_t width;
	uint16_t start_x;
	uint16_t start_y;
	uint16_t radius;
	uint16_t end_x;
	uint16_t end_y;
	int16_t start_angle;
	int16_t end_angle;
	uint8_t text_lenght;
	uint8_t text[30];
} __attribute__((packed))ext_client_graphic_draw_t;

typedef enum {
		NULL_OPERATE = 0U, /* using in add char */
    ADD_PICTURE = 1U,
    MODIFY_PICTURE = 2U,
    CLEAR_ONE_PICTURE = 3U,
    CLEAR_ONE_LAYER = 4U,
    CLEAR_ALL = 5U,
} drawOperate_e;

typedef enum
{
	RED = 0U,
	BLUE = 0U,
	YELLOW,
	GREEN,
	ORANGE,
	PURPLE,
	PINK,
	DARKGREEN,
	BLACK,
	WHITE
}colorType_e;

class referee_Classdef
{
	public:
		/* 裁判系统数据 */
		ext_game_status_t GameState;
		ext_game_robot_HP_t GameRobotHP;
		ext_event_data_t EventData;
		ext_game_robot_status_t GameRobotState;
		ext_power_heat_data_t PowerHeatData;
		ext_robot_hurt_t RobotHurt;
		ext_shoot_data_t ShootData;
		ext_bullet_remaining_t BulletRemaining;
		ext_student_interactive_header_data_t StudentInteractiveHeaderData;
		robot_interactive_data_t RobotInteractiveData;
		RC_ID robot_client_ID;
	
		RF_status_e status;
	
		referee_Classdef(){

		}
		
		void Set_refereeUart(UART_HandleTypeDef *_huart);
	
		void unPackDataFromRF(uint8_t *data_buf, uint32_t length);
		void CV_ToOtherRobot(uint8_t target_id, uint8_t data1);
		void CV_ToServer(float data1,float data2,float data3, uint8_t light, uint8_t color);
		uint8_t RobotInteractiveHandle(robot_interactive_data_t* RobotInteractiveData_t);
		
		void Set_DrawingLayer(uint8_t _layer);
		void line_drawing(drawOperate_e _operate_type,uint16_t startx,uint16_t starty,uint16_t endx,uint16_t endy,colorType_e vcolor,uint8_t name[]);
		void rectangle_drawing(drawOperate_e _operate_type, uint16_t startx,uint16_t starty,uint16_t length_,uint16_t width_,colorType_e vcolor, uint8_t name[]);
		void circle_drawing(drawOperate_e _operate_type, uint16_t centrex,uint16_t centrey,uint16_t r,colorType_e vcolor, uint8_t name[]);
		void oval_drawing(drawOperate_e _operate_type, uint16_t centrex,uint16_t centrey,uint16_t minor_semi_axis,uint16_t major_semi_axis,colorType_e vcolor, uint8_t name[]);
		void arc_drawing(drawOperate_e _operate_type, uint16_t centrex,uint16_t centrey,uint16_t endx,uint16_t endy,int16_t start_angle_,int16_t end_angle_,colorType_e vcolor, uint8_t name[]);
		void character_drawing(drawOperate_e _operate_type,uint16_t startx,uint16_t starty,uint16_t size, uint8_t length,uint8_t character[], colorType_e vcolor, uint8_t name[]);
		void clean_one_picture(uint8_t vlayer,uint8_t name[]);
		void clean_layer(uint8_t _layer);
		void clean_all();
		
		uint8_t UI_ruler(uint32_t _sys_time, uint8_t ruler_tag, uint8_t sacle_num = 5, uint16_t start_x = 960, uint16_t start_y = 540 - 24, uint16_t step = 40, uint16_t scale_long = 60, uint16_t scale_short = 32, colorType_e _color = WHITE);

	private:
		UART_HandleTypeDef *refereeUart;
		ext_client_graphic_draw_t drawing;

		void Calc_Robot_ID(uint8_t local_id);
		void SendDrawData();
		
		/* 解包过程用到的变量及函数 */
		uint8_t DataCheck(uint8_t **p);
		unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
		uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
		void RefereeHandle(uint8_t *data_buf);
};

extern uart_pack_s uart_pack;
uint32_t unpack_UI(uint8_t *data_buf,uint16_t length);

#endif
#endif
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
