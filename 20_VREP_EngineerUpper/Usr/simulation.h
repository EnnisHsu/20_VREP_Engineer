#pragma once
#include <System_DataPool.h>
#include <CoppeliaSim.h>
extern CoppeliaSim_Client CoppeliaSim;
extern TaskHandle_t Sim_Handle;

void Task_Sim(void* arg);
void Service_CoppeliaSim_Init();
