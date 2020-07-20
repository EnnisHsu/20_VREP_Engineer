#ifndef _VSEC_H
#define _VSEC_H

#include "stdint.h"
#include "drv_can.h"

//CAN 发送的标志位   还有其他接口但是没有给出  所以请勿发送超出下面的标志位
//如果超出标志位 驱动器也会执行 但是会产生意外错误
typedef enum {
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,
	CAN_PACKET_SET_CURRENT_BRAKE,
	CAN_PACKET_SET_RPM,
	CAN_PACKET_SET_POS,
	CAN_PACKET_SET_PLANNING_POS,
	CAN_PACKET_SET_RESETPOS,
	CAN_PACKET_SET_CURRENT_REL,
	CAN_PACKET_SET_CURRENT_BRAKE_REL,
	CAN_PACKET_SET_CURRENT_HANDBRAKE,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
	CAN_PACKET_STATUS,
} CAN_PACKET_ID;

void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
	buffer[(*index)++] = number >> 24;
	buffer[(*index)++] = number >> 16;
	buffer[(*index)++] = number >> 8;
	buffer[(*index)++] = number;
}

/* 设置电流值 单位A */
void VSEC_Set_Current(CAN_HandleTypeDef *hcan,uint8_t controller_id, float current) {
	int32_t send_index = 0;
	uint8_t buffer[4];
	buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
	CANx_SendExtData(hcan,(controller_id|((uint32_t)CAN_PACKET_SET_CURRENT<<8)),buffer,8);
}

#endif
