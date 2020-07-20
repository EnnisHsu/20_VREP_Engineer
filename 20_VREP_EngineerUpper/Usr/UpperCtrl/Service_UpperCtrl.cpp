#include <iostream>
//#include "inputInterface.h"
#include<conio.h>
#include<Windows.h>
#include<WinUser.h>
#include "User_Task.h"
#include "Service_UpperCtrl.h"
#include "simulation.h"
#include "extApi.h"
#include "UpperMonitor.h"
#include "win32_support.h"
#include "System_DataPool.h"





#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

BulletStatusTypedef Bullet_Status;

BulletTaskTypedef Bullet_Task_Status;

TaskHandle_t KeyCmd_TaskHandle, Target_TaskHandle;

float Slide_Target, Stretch_Target;
float Slide_Current, Stretch_Current;

void Service_UpperCtrl_Init()
{
	xTaskCreate(KeyCmd_Handle, "KeyCmd_Handle", Large_Stack_Size, NULL, PriorityHigh, &KeyCmd_TaskHandle);
	xTaskCreate(Target_Handle, "Target_Handle", Large_Stack_Size, NULL, PriorityHigh, &Target_TaskHandle);
}

void Lift_Top()
{
	Joint[_LF][Lift]->obj_Target.angle_f = 0.25f;
}

void Lift_Med()
{
	Joint[_LF][Lift]->obj_Target.angle_f = 0.17f;
}

void Lift_Btm()
{
	Joint[_LF][Lift]->obj_Target.angle_f = 0.0f;
}

void Slide_Left()
{
	//Joint[_LF][Slide]->obj_Target.angle_f = 0.25f;
	Slide_Target = 0.28f;
}

void Slide_Med()
{
	//Joint[_LF][Slide]->obj_Target.angle_f = 0.0f;
	Slide_Target = 0.0f;
}

void Slide_Right()
{
	//Joint[_LF][Slide]->obj_Target.angle_f = -0.25f;
	Slide_Target = -0.28f;
}

void Rotate()
{
	Joint[_LF][Turnover]->obj_Target.angle_f = 3.14f;
}

void RotateBack()
{
	Joint[_LF][Turnover]->obj_Target.angle_f = 0.8f;
}

void RotateAllBack()
{
	Joint[_LF][Turnover]->obj_Target.angle_f = 0.0f;
}

void StretchOut()
{
	//Joint[_RF][Extended]->obj_Target.angle_f = 0.24f;
	Stretch_Target = 0.24f;
}
void StretchBack()
{
	//Joint[_RF][Extended]->obj_Target.angle_f = 0.0f;
	Stretch_Target = 0.0f;
}

void SecStretchOut()
{
	Joint[_LF][Extended]->obj_Target.angle_f = 0.05f;
}

void SecStretchBack()
{
	Joint[_LF][Extended]->obj_Target.angle_f = 0.00f;
}

void Pinch()
{
	Joint[_LF][Clamp]->obj_Target.angle_f = -0.035f;
	Joint[_RF][Clamp]->obj_Target.angle_f = -0.035f;
}

void Loosen()
{
	Joint[_LF][Clamp]->obj_Target.angle_f = 0.0f;
	Joint[_RF][Clamp]->obj_Target.angle_f = 0.0f;
}

void SetDummyLink(simxInt DummyLinkStatus)
{
	ClampDummyStatus->target = DummyLinkStatus;
	SysLog.Record(_INFO_, "CoppeliaSim", "DummyLinkStatus %d", DummyLinkStatus);
}

void EngineerInit()
{
	Loosen();
	RotateAllBack();
	StretchBack();
	Slide_Med();
	Lift_Btm();
}


/*void Task_RemoteControl(void* arg)
{
	TickType_t xLastWakeTime_Remote;
	char ch;
	EngineerInit();
	for (;;)
	{
		xLastWakeTime_Remote = xTaskGetTickCount();
		ch = '\0';
		if (GetKeyState(VK_UP) < 0)
		{
			EngineerForward();
		}
		else
		{
			if (GetKeyState(VK_DOWN) < 0)
			{
				EngineerBack();
			}
			else
			{
				if (GetKeyState(VK_LEFT) < 0)
				{
					EngineerLeft();
				}
				else
				{
					if (GetKeyState(VK_RIGHT) < 0)
					{
						EngineerRight();
					}
					else
					{
						EngineerStop();
					}
				}
			}
		}
		if (GetKeyState(VK_LCONTROL) < 0)
		{
			if (_kbhit)
			{
				ch = getch();
				std::cout << "Ctrl_" << (char)ch << "pressed\n";
				switch (ch)
				{
				case 26:
					Slide_Left();
					StretchOut();
					break;
				case 24:
					Slide_Med();
					StretchOut();
					break;
				case 3:
					Slide_Right();
					StretchOut();
					break;
				default:
					break;
				}

			}
		}
		else
			if (_kbhit())
			{
				ch = getch();
				std::cout << "Key_" << (char)ch << " pressed\n";
				switch (ch)
				{
				case 'z':
					Slide_Left();
					StretchBack();
					//std::cout << "Key_" << (char)ch << " pressed\n";
					//SysLog.Record(_INFO_, "Interface", "Key_Z pressed");
					break;
				case 'x':
					Slide_Med();
					StretchBack();
					//std::cout << "Key_" << (char)ch << " pressed\n";
					break;
				case 'c':
					Slide_Right();
					StretchBack();
					//std::cout << "Key_" << (char)ch << " pressed\n";
					break;
				case 'r':
					Lift_Top();
					//std::cout << "Key_" << (char)ch << " pressed\n";
					break;
				case 'f':
					Lift_Med();
					//std::cout << "Key_" << (char)ch << " pressed\n";
					break;
				case 'v':
					Lift_Btm();
					//std::cout << "Key_" << (char)ch << " pressed\n";
					break;
				case 't':
					Rotate();
					//std::cout << "Key_" << (char)ch << " pressed\n";
					break;
				case 'y':
					RotateAllBack();
					//std::cout << "Key_" << (char)ch << " pressed\n";
					break;
				case 'u':
					StretchOut();
					//std::cout << "Key_" << (char)ch << " pressed\n";
					break;
				case 'i':
					StretchBack();
					//std::cout << "Key_" << (char)ch << " pressed\n";
					break;
				case 'o':
					Pinch();
					break;
				case 'p':
					Loosen();
					break;
				case 'b':
					Bullet_Status = Auto_Reset;
					//std::cout << "Key_" << (char)ch << " pressed\n";
					break;
				case 'g':
					Bullet_Task_Status = Task_Auto_1Box;
					Bullet_Status = Lifting;
					std::cout << "Task Auto 1Box Start\n";
					break;
				case 'h':
					Bullet_Task_Status = Task_Auto_2Boxes_of_One;
					Bullet_Status = Lifting;
					std::cout << "Task Auto 2Box Start\n";
					break;
				case 'n':
					SetDummyLink(1);
					break;
				case 'm':
					SetDummyLink(0);
					break;
				default:
					break;
				}


			}


		CoppeliaSim.ComWithServer();
		vTaskDelayUntil(&xLastWakeTime_Remote, 1);
	}
}*/

void KeyCmd_Handle(void* arg)
{
	TickType_t xLastWakeTime_KeyCmd;
	int flag;

	for (;;)
	{
		xLastWakeTime_KeyCmd = xTaskGetTickCount();
		switch (Bullet_Status)
		{
		case WaitingBullet:
			flag = 1;
			break;
		case Lifting:
			Lift_Med();
			if (flag)
			{
				std::cout << "Lifting\n";
				flag = 0;
			}
			//				Bullet_Status = SlideBullet; 
			if ((Joint[_LF][Lift]->obj_Data.angle_f >= 0.16) && (Joint[_LF][Lift]->obj_Data.angle_f <= 0.18))
			{
				flag = 1;
				switch (Bullet_Task_Status)
				{
				case Task_Auto_1Box:
					Bullet_Status = RotateClaw;
					break;
				case Task_Auto_2Boxes_of_One:
					Bullet_Status = SlideLeftBullet;
					break;
				case Task_Auto_2Boxes_of_Two:
					Bullet_Status = SlideRightBullet;
					break;
				case Task_Auto_4Boxes_of_One:
					Bullet_Status = RotateClaw;
					break;
				case Task_Auto_4Boxes_of_Two:
					Bullet_Status = SlideLeftBullet;
					break;
				case Task_Auto_4Boxes_of_Thr:
					Bullet_Status = SlideMedBullet;
					break;
				case Task_Auto_4Boxes_of_For:
					Bullet_Status = SlideRightBullet;
					break;
				default:
					break;
				}
			}
			break;
		case SlideLeftBullet:
			Slide_Left();
			if (flag)
			{
				std::cout << "SlideLeft\n";
				flag = 0;
			}
			switch (Bullet_Task_Status)
			{
				case Task_Auto_2Boxes_of_One:
					if (Joint[_LF][Slide]->obj_Data.angle_f >= 0.27f)
					{
						Bullet_Status = RotateClaw;
						flag = 1;
					}
					break;
				case Task_Auto_4Boxes_of_Two:
					StretchOut();
					SecStretchOut();
					if (Joint[_LF][Slide]->obj_Data.angle_f >= 0.27f && Joint[_RF][Extended]->obj_Data.angle_f >= 0.23f)
					{
						Bullet_Status = RotateClaw;
						flag = 1;
					}
					break;
				default:
					break;
			}
			
			break;
		case SlideMedBullet:
			Slide_Med();
			if (flag)
			{
				std::cout << "SlideMedRight\n";
				flag = 0;
			}
			StretchOut();
			SecStretch();
			if (Joint[_RF][Extended]->obj_Data.angle_f >= 0.23f && abs(Joint[_LF][Slide]->obj_Data.angle_f) <= 0.01f)
			{
				Bullet_Status = RotateClaw;
				flag = 1;
			}
			break;
		case SlideRightBullet:
			Slide_Right();
			if (flag)
			{
				std::cout << "SlideRight\n";
				flag = 0;
			}
			switch (Bullet_Task_Status)
			{
				case Task_Auto_2Boxes_of_Two:
					if (Joint[_LF][Slide]->obj_Data.angle_f <= -0.27f)
					{
						Bullet_Status = RotateClaw;
						flag = 1;
					}
					break;
				case Task_Auto_4Boxes_of_For:
					StretchOut();
					SecStretchOut();
					if (Joint[_LF][Slide]->obj_Data.angle_f <= -0.27f && Joint[_RF][Extended]->obj_Data.angle_f >= 0.23f)
					{
						Bullet_Status = RotateClaw;
						flag = 1;
					}
					break;
				default:
					break;
			}
			
			break;
		case Stretch:
			StretchOut();
			SecStretchOut();
			if (Joint[_RF][Extended]->obj_Data.angle_f >= 0.23f) Bullet_Status = RotateClaw;
			break;
		case RotateClaw:
			Rotate();
			Bullet_Status = RotatingClaw;
			break;
		case RotatingClaw:
			if (Joint[_LF][Turnover]->obj_Data.angle_f > 1.2f)
			{
				SetDummyLink(0);
				Loosen();
				vTaskDelay(500);
				if (flag)
				{
					std::cout << "Loosen\n";
					flag = 0;
				}
				if (Joint[_LF][Turnover]->obj_Data.angle_f >= 3.1f)
				{
					Bullet_Status = PinchBullet;
					flag = 1;
				}
			}
			break;
		case PinchBullet:

			Pinch();
			vTaskDelay(500);
			switch (Bullet_Task_Status)
			{
			case Task_Auto_1Box:
				SetDummyLink(1);
				break;
			case Task_Auto_2Boxes_of_One:
				SetDummyLink(2);
				break;
			case Task_Auto_2Boxes_of_Two:
				SetDummyLink(3);
				break;
			case Task_Auto_4Boxes_of_One:
				SetDummyLink(1);
				break;
			case Task_Auto_4Boxes_of_Two:
				SetDummyLink(4);
				break;
			case Task_Auto_4Boxes_of_Thr:
				SetDummyLink(5);
				break;
			case Task_Auto_4Boxes_of_For:
				SetDummyLink(6);
				break;
			default:
				break;
			}
			vTaskDelay(200);
			Bullet_Status = PinchEnd;
			break;
		case PinchEnd:
			Lift_Top();
			if (flag)
			{
				std::cout << "PinchEnd\n";
				flag = 0;
			}
			vTaskDelay(500);
			switch (Bullet_Task_Status)
			{
				case Task_Auto_1Box:
				case Task_Auto_2Boxes_of_One:
				case Task_Auto_2Boxes_of_Two:
				case Task_Auto_4Boxes_of_One:
					if ((Joint[_LF][Lift]->obj_Data.angle_f > ROTATE_START_POS))
					{
						Bullet_Status = TakebackBullet;
						flag = 1;
					}
					break;
				case Task_Auto_4Boxes_of_Two:
				case Task_Auto_4Boxes_of_Thr:
				case Task_Auto_4Boxes_of_For:
					StretchBack();
					SecStretchBack();
					if ((Joint[_LF][Lift]->obj_Data.angle_f > ROTATE_START_POS) && (Joint[_RF][Extended]->obj_Data.angle_f <= 0.01f))
					{
						vTaskDelay(200);
						Bullet_Status = TakebackBullet;
						flag = 1;
					}
					break;
			}
			//				vTaskDelayUntil(&xLastWakeTime_KeyCmd,2);
			break;
		case TakebackBullet:
			RotateBack();
			if (flag) std::cout << "RotateBack\n";
			vTaskDelay(1000);
			Bullet_Status = TakebackEnd;
			flag = 1;
			break;
		case TakebackEnd:
			switch (Bullet_Task_Status)
			{
			case Task_Auto_1Box:
				std::cout << "Task Auto 1Box End\n";
				Bullet_Task_Status = Waiting_Task;
				Bullet_Status = WaitingBullet;
				break;
			case Task_Auto_2Boxes_of_One:
				Bullet_Task_Status = Task_Auto_2Boxes_of_Two;
				std::cout << "Task Auto 2Boxs of One End\n";
				Bullet_Status = Lifting;
				break;
			case Task_Auto_2Boxes_of_Two:
				std::cout << "Task Auto 2Boxs End Auto Reset now\n";
				Bullet_Status = Auto_Reset;
				break;
			case Task_Auto_4Boxes_of_One:
				std::cout << "Task_Auto_4Boxes_of_One End\n";
				Bullet_Task_Status = Task_Auto_4Boxes_of_Two;
				Bullet_Status = Lifting;
				break;
			case Task_Auto_4Boxes_of_Two:
				Bullet_Task_Status = Task_Auto_4Boxes_of_Thr;
				std::cout << "Task_Auto_4Boxes_of_Two End\n";
				Bullet_Status = Lifting;
				break;
			case Task_Auto_4Boxes_of_Thr:
				Bullet_Task_Status = Task_Auto_4Boxes_of_For;
				std::cout << "Task_Auto_4Boxes_of_Thr End\n"; 
				Bullet_Status = Lifting;
				break;
			case Task_Auto_4Boxes_of_For:
				std::cout << "Task_Auto_4Boxes_of_For End Auto Reset Now\n";
				Bullet_Status = Auto_Reset;
				break;
			default:
				break;
			}
			break;
		case Auto_Reset:
			RotateBack();
			Slide_Med();
			if ((abs(Joint[_LF][Slide]->obj_Data.angle_f) <= 0.01))
				Bullet_Status = Slave_Reset;
			break;
		case Slave_Reset:
			Lift_Med();
			Bullet_Status = Slave_Reseting;
			break;
		case Slave_Reseting:
			if ((Joint[_LF][Lift]->obj_Data.angle_f >= 0.16) && (Joint[_LF][Lift]->obj_Data.angle_f <= 0.18))
			{
				std::cout << "Auto Reset End\n";
				Bullet_Status = WaitingBullet;
				Bullet_Task_Status = Waiting_Task;
			}
			break;
		default:
			break;
		}
		vTaskDelayUntil(&xLastWakeTime_KeyCmd, 1);
	}
}

point originPoint[PointCount];

void Target_Handle(void* arg)
{
	TickType_t xLastWakeTime_Target;
	float offset;
	xLastWakeTime_Target = xTaskGetTickCount();
	for (;;)
	{
		//if (abs(Slide_Target-Joint[_LF][Slide]->obj_Data.angle_f)<1e-02 || abs(Stretch_Target-Joint[_RF][Extended]->obj_Data.angle_f)<1e-02)
		//{
		//	//memset(originPoint, 0, sizeof(originPoint));
		//	originPoint[0].x = Joint[_LF][Slide]->obj_Data.angle_f;
		//	originPoint[0].y = Joint[_RF][Extended]->obj_Data.angle_f;
		//	originPoint[1].x = Slide_Target;
		//	originPoint[1].y = Stretch_Target;
		//	createCurve(originPoint,2);
		//}
		if (Stretch_Target == Stretch_Current) offset = 0;
		else offset = Stretch_Current * Slide_Target - Slide_Current * Stretch_Target;
		if (offset == 0 && Stretch_Current == Stretch_Target && Slide_Current == Slide_Target)
		{

		}
		else
		{
			if (Slide_Target > Slide_Current)
			{
				if (offset >= 0)	Slide_Current = min(Slide_Current + 0.01, Slide_Target);
				else Stretch_Current = min(Stretch_Current + 0.01, Stretch_Target);
			}
			if (Slide_Target < Slide_Current)
			{
				if (offset >= 0)	Slide_Current = max(Slide_Current - 0.01, Slide_Target);
				else Stretch_Current = min(Stretch_Current + 0.01, Stretch_Target);
			}
			if (Slide_Target == Slide_Current)
			{ 
				if (Stretch_Target > Stretch_Current)
				{
					Stretch_Current = min(Stretch_Current + 0.01, Stretch_Target);
				}
				if (Stretch_Target < Stretch_Current)
				{
					Stretch_Current = max(Stretch_Current - 0.01, Stretch_Target);
				}
				//if (Slide_Current == 0) stretch = 

			}
		}
		Joint[_LF][Slide]->obj_Target.angle_f = Slide_Current;
		Joint[_RF][Extended]->obj_Target.angle_f = Stretch_Current;

		vTaskDelayUntil(&xLastWakeTime_Target, 1);
	}
}


void createCurve(point* originPoint, int originCount) {
	//控制点收缩系数 ，经调试0.6较好，CvPoint是opencv的，可自行定义结构体(x,y)
	float scale = 0.6;
	point* midpoints = (point*)malloc(sizeof(point) * originCount);
	//生成中点     
	for (int i = 0; i < originCount; i++) {
		int nexti = (i + 1) % originCount;
		midpoints[i].x = (originPoint[i].x + originPoint[nexti].x) / 2.0;
		midpoints[i].y = (originPoint[i].y + originPoint[nexti].y) / 2.0;
	}

	//平移中点
	point* extrapoints = (point*)malloc(sizeof(point) * 2*originCount);
	for (int i = 0; i < originCount; i++) {
		int nexti = (i + 1) % originCount;
		int backi = (i + originCount - 1) % originCount;
		point midinmid;
		midinmid.x = (midpoints[i].x + midpoints[backi].x) / 2.0;
		midinmid.y = (midpoints[i].y + midpoints[backi].y) / 2.0;
		float offsetx = originPoint[i].x - midinmid.x;
		float offsety = originPoint[i].y - midinmid.y;
		int extraindex = 2 * i;
		extrapoints[extraindex].x = midpoints[backi].x + offsetx;
		extrapoints[extraindex].y = midpoints[backi].y + offsety;
		//朝 originPoint[i]方向收缩 
		float addx = (extrapoints[extraindex].x - originPoint[i].x) * scale;
		float addy = (extrapoints[extraindex].y - originPoint[i].y) * scale;
		extrapoints[extraindex].x = originPoint[i].x + addx;
		extrapoints[extraindex].y = originPoint[i].y + addy;

		int extranexti = (extraindex + 1) % (2 * originCount);
		extrapoints[extranexti].x = midpoints[i].x + offsetx;
		extrapoints[extranexti].y = midpoints[i].y + offsety;
		//朝 originPoint[i]方向收缩 
		addx = (extrapoints[extranexti].x - originPoint[i].x) * scale;
		addy = (extrapoints[extranexti].y - originPoint[i].y) * scale;
		extrapoints[extranexti].x = originPoint[i].x + addx;
		extrapoints[extranexti].y = originPoint[i].y + addy;

	}

	point controlPoint[4];
	//生成4控制点，产生贝塞尔曲线
	for (int i = 0; i < originCount; i++) {
		controlPoint[0] = originPoint[i];
		int extraindex = 2 * i;
		controlPoint[1] = extrapoints[extraindex + 1];
		int extranexti = (extraindex + 2) % (2 * originCount);
		controlPoint[2] = extrapoints[extranexti];
		int nexti = (i + 1) % originCount;
		controlPoint[3] = originPoint[nexti];
		float u = 1;
		while (u >= 0) {
			float px = bezier3funcX(u, controlPoint);
			float py = bezier3funcY(u, controlPoint);
			//u的步长决定曲线的疏密
			u -= 0.005;
			std::cout << px << "," << py << "\n";
			Joint[_LF][Slide]->obj_Target.angle_f = px;
			Joint[_RF][Extended]->obj_Target.angle_f = py;
		}
	}
	free(midpoints);
	free(extrapoints);
}
//三次贝塞尔曲线
float bezier3funcX(float uu, point* controlP) {
	float part0 = controlP[0].x * uu * uu * uu;
	float part1 = 3 * controlP[1].x * uu * uu * (1 - uu);
	float part2 = 3 * controlP[2].x * uu * (1 - uu) * (1 - uu);
	float part3 = controlP[3].x * (1 - uu) * (1 - uu) * (1 - uu);
	return part0 + part1 + part2 + part3;
}
float bezier3funcY(float uu, point* controlP) {
	float part0 = controlP[0].y * uu * uu * uu;
	float part1 = 3 * controlP[1].y * uu * uu * (1 - uu);
	float part2 = 3 * controlP[2].y * uu * (1 - uu) * (1 - uu);
	float part3 = controlP[3].y * (1 - uu) * (1 - uu) * (1 - uu);
	return part0 + part1 + part2 + part3;
}

