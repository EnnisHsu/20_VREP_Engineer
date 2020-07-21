#include "simulation.h"
#include <iostream>
#include "System_DataPool.h"
using namespace std;
TaskHandle_t Sim_Handle;
#define SYNC_MODE
void Service_CoppeliaSim_Init()
{
#ifndef SYNC_MODE
  if (CoppeliaSim->Start("127.0.0.1", 5000, 5))
#else
  if (CoppeliaSim->Start("127.0.0.1", 5000, 5, true))// 
#endif
    SysLog.Record(_INFO_, "CoppeliaSim", "CoppeliaSim connection initialized successfully.");
  else
  {
    SysLog.Record(_INFO_, "CoppeliaSim", "CoppeliaSim failed to connect.");
    SysLog.Record(_INFO_, "CoppeliaSim", "Waiting for reconnecting...");
  }
#ifndef SYNC_MODE
  xTaskCreate(Task_Sim, "CoppeliaSim service", Huge_Stack_Size, NULL, PriorityHigh, &Sim_Handle);
#endif
}


void Task_Sim(void* arg)
{
  /* Cache for Task */
  TickType_t xLastWakeTime_t;
  const TickType_t xBlockTime = pdMS_TO_TICKS(5);
  /* Pre-Load for task */
  xLastWakeTime_t = xTaskGetTickCount();
  (void)arg;
  /* Infinite loop */
  for (;;)
  {
    if (CoppeliaSim->ComWithServer())
      SysLog.Record(_INFO_, "Simulation", "Simulation is running !");  
    else
      SysLog.Record(_INFO_, "Simulation", "Simulation is stopped !");

    /* Pass control to the next task */
    vTaskDelayUntil(&xLastWakeTime_t, xBlockTime);
  }
}
