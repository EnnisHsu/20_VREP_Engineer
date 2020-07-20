#ifndef _COMMUCATION_H_
#define _COMMUCATION_H_

#ifdef __cplusplus
#include "SRML.h"
extern "C"
{
#endif

#define ROTATE_START_POS 0.23f

	enum BulletStatusTypedef {
		WaitingBullet,
		Lifting,
		SlideLeftBullet,
		SlideMedBullet,
		SlideRightBullet,
		Stretch,
		Retract,
		RotateClaw,
		RotatingClaw,
		PinchBullet,
		PinchEnd,
		TakebackBullet,
		TakebackEnd,
		Auto_Reset,
		Upper_Reset_End,
		Slave_Reset,
		Slave_Reseting,
	};

	enum BulletTaskTypedef {
		Waiting_Task,
		Task_Auto_1Box,
		Task_Auto_2Boxes_of_One,
		Task_Auto_2Boxes_of_Two,
		Task_Auto_4Boxes_of_One,
		Task_Auto_4Boxes_of_Two,
		Task_Auto_4Boxes_of_Thr,
		Task_Auto_4Boxes_of_For,
	};

	struct point {
		float x, y;
	};

	#define PointCount 100

	extern BulletStatusTypedef Bullet_Status;

	extern BulletTaskTypedef Bullet_Task_Status;
	extern float Slide_Target, Stretch_Target;
	extern float Slide_Current, Stretch_Current;

	void Service_UpperCtrl_Init();

	void Lift_Top();
	void Lift_Med();
	void Lift_Btm();
	void Slide_Left();
	void Slide_Med();
	void Slide_Right();
	void Rotate();
	void RotateBack();
	void RotateAllBack();
	void StretchOut();
	void StretchBack();
	void Pinch();
	void Loosen();
	void SetDummyLink(simxInt DummyLinkStatus);
	void EngineerInit();

	void KeyCmd_Handle(void* arg);

	void Target_Handle(void* arg);

	void createCurve(point* originPoint, int originCount);
	float bezier3funcX(float uu, point* controlP);
	float bezier3funcY(float uu, point* controlP);



#ifdef __cplusplus
}
#endif
#endif

