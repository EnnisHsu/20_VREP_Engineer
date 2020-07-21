#include "CoppeliaSim_service.h"
#include <mutex>
#include "System_DataPool.h"
#define SYNC_MODE
static std::mutex g_com_state_mtx;   /* g_com_state的互斥锁  */ 
static int g_com_state = 0;          /* 通信状态位，在transmit和通信线程中更改与查询 */
                                     /* 1:通信完成 0：开始通信*/
/**
 * @brief Used in test for CoppeliaSim Client.
 * @note  Start instance thread of coppeliasim-client, using 127.0.0.1:5000, Synchronous
          Mode is enabled.
 * 
 */
void CoppeliaSim_Service()
{
   // TickType_t xLastWakeTime_t;
 //   const TickType_t xBlockTime = pdMS_TO_TICKS(5);
    /* Pre-Load for task */
  //  xLastWakeTime_t = xTaskGetTickCount();
  //  (void)arg;
    /* Infinite loop */
    for (;;)
    {
        if (CoppeliaSim->ComWithServer())
            SysLog.Record(_INFO_, "Simulation", "Simulation is running !");
        else
            SysLog.Record(_INFO_, "Simulation", "Simulation is stopped !");

        /* Pass control to the next task */
        Sleep(5);
      //  vTaskDelayUntil(&xLastWakeTime_t, xBlockTime);
    }



  /*  std::cout << "[CoppeliaSim Client] Successfully created service thread ! \n";
    CoppeliaSim_Client *hClient = &CoppeliaSim_Client::getInstance();
    std::cout << "[CoppeliaSim Client] Connecting to server.. \n";*/
    /*Make sure to connect*/
    //hClient->Set_startParam("127.0.0.1", 5000, 5, true);
    //while (!hClient->Start()){};//"127.0.0.1", 5000, 5, true
    //std::cout << "[CoppeliaSim Client] Successfully connected to server !\n";

    /*Infinite loop*/
    /*for (;;)
    {
      if (Get_com_state() == 0)
      {
        if (CoppeliaSim->Is_Connected())
        {
            CoppeliaSim->ComWithServer();
        }
        Set_com_state(1);
      }
    }*/
}

void Coppeliasim_Client_Init()
{
#ifndef SYNC_MODE
    if (CoppeliaSim->Start("127.0.0.1", 5000, 5))
#else
    if (CoppeliaSim->Start("127.0.0.1", 5000, 5, true))
#endif
        SysLog.Record(_INFO_, "CoppeliaSim", "CoppeliaSim connection initialized successfully.");
    else
    {
        SysLog.Record(_INFO_, "CoppeliaSim", "CoppeliaSim failed to connect.");
        SysLog.Record(_INFO_, "CoppeliaSim", "Waiting for reconnecting...");
    }


  //CoppeliaSim_Client* hClient = &CoppeliaSim_Client::getInstance();
 // std::cout << "[CoppeliaSim Client] Initializing service, setting parameters... \n";
 // std::cout << "[CoppeliaSim Client] Creating client service thread... \n";
  std::thread sim_thread(CoppeliaSim_Service);
  sim_thread.detach();
}

int Get_com_state() {
  return g_com_state;
}

void Set_com_state(int _com_state) {

  while (!g_com_state_mtx.try_lock());
  g_com_state = _com_state;
  g_com_state_mtx.unlock();
}


/************************ END-OF-FILE SCUT-ROBOTLAB **************************/