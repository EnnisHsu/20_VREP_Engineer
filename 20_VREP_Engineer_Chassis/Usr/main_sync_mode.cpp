
#define SYNC_MODE
#ifdef SYNC_MODE
#include "System_Config.h"
#include "simulation.h"
#include "Remote_Control.h"
#include "Service_Engineer.h"

extern void Task_RemoteControl(void* arg);
extern void   Task_EngineerControl(void* arg);
extern void Tsak_RobotCtrl(void* arg);
extern void Task_DataDisplay(void* arg);

int main(void)
{
	//SYSTEMTIME sys; 
	//GetLocalTime(&sys);
	//printf("Begin:\t%4d/%02d/%02d %02d:%02d:%02d.%03d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	System_Resource_Init();
	System_Tasks_Init();
	while (1)
	{
		while (!CoppeliaSim->ComWithServer())
			SysLog.Record(_INFO_, "Simulation", "Simulation error !");
		Task_RemoteControl((void*)0);
		Task_EngineerControl((void*)0);
		Tsak_RobotCtrl((void*)0);
		Task_DataDisplay((void*)0);
	}
	return 0;
}

#endif