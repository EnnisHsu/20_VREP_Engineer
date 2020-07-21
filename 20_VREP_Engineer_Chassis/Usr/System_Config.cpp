/**
  ******************************************************************************
  * @file   System_config.cpp
  * @brief  Deploy resources,tasks and services in this file.
  ******************************************************************************
  * @note
  *  - Before running your Task you should first include your headers and init- 
  *    ialize used resources in "System_Resource_Init()". This function will be 
  *    called before tasks Start.
  *    
  *  - All tasks should be created in "System_Tasks_Init()".   
  *  - FreeRTOS scheduler will be started after tasks are created.
  *
  ===============================================================================
                                    Task List
  ===============================================================================
  * <table>
  * <tr><th>Task Name     <th>Priority          <th>Frequency/Hz    <th>Stack/Byte
  * <tr><td>              <td>                  <td>                <td>    
  * </table>
  *
 */
/* Includes ------------------------------------------------------------------*/
#include <iostream>
#include "inputInterface.h"
#include "System_Config.h"
#include "System_DataPool.h"
#include "Service_Engineer.h"
#include "Remote_Control.h"
/* Service */
//#include "simulation.h"
#include "win32_support.h"
#include "CoppeliaSim_service.h"
/* User support package & SRML */
#include "User_Task.h"
#include "UpperMonitor.h"
#include <Usr\simulation.h>
/* Private variables ---------------------------------------------------------*/
extern int COM_Callback(unsigned char* data, unsigned int num);

/*Founctions------------------------------------------------------------------*/
/**
* @brief Load drivers ,modules, and data resources for tasks.
* @note  Edit this function to add Init-functions and configurations.
*/
void System_Resource_Init(void)
{
  std::cout << 
    "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \r\n"
    "*                                                                                   * \r\n"
    "*                     Robomaster Development Simulation Platform                    * \r\n"
    "*                          Supported By Coppeliasim Robotic                         * \r\n"
    "*                                                                                   * \r\n"
    "*                            COPYRIGHT(C) SCUT-ROBOTLAB                             * \r\n"
    "*                                                                                   * \r\n"
    "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \r\n" ;
  /* Drivers Init ---------------------*/

  /* RTOS resources Init --------------*/

  /* Other resources Init -------------*/
  LogOutputBacken_Init();

  /* Modules Init ---------------------*/
  
  /* Service configurations -----------*/
  SysLog.getMilliTick_regist(xTaskGetTickCount);
  SysLog.filter_conf(DEFAULT_TAG, LOG_LVL_ALL, LogConsole_Output);
  SysLog.filter_conf("Simulation", LOG_LVL_INFO, LogConsole_Output, &Filter_List[0]);
  SysLog.filter_conf("CoppeliaSim", LOG_LVL_INFO, LogConsole_Output, &Filter_List[1]);
  SysLog.global_conf(LOG_LVL_ALL, false);
  SysLog.Record(_INFO_, "SysLog initialized success...");

  SerialPort.Register_RecvCallBack(COM_Callback);

}  


/**
* @brief Load and start User Tasks. This function run directly in "main()"
* @note  Edit this function to add tasks into the activated tasks list.
*/
void System_Tasks_Init(void)
{
  /* Syetem Service init --------------*/
  /* KeyBord & Mouse (Developed by MFC) Service */
  SysLog.Record(_INFO_, "Initializing Keyboard and mouse interfaces...");
 // keyboard.init();
//	mouse.init();

  /*Serial Communication service*/
  SysLog.Record(_INFO_, "Initializing communication service...");
  Service_SerialPortCom_Init(1);

  /*CoppeliaSim service*/
  SysLog.Record(_INFO_, "Initializing CoppeliaSim simulation service...");
 // Coppeliasim_Client_Init();
 Service_CoppeliaSim_Init();
  //Mecanum_wheel_2
  Body = CoppeliaSim->Add_Object("chassis_respondable", OTHER_OBJECT, { SIM_POSITION | CLIENT_RO, SIM_VELOCITY | CLIENT_RO });//chassis_respondable
  Joint[_LF][Wheel] = CoppeliaSim->Add_Object("LF", JOINT, { SIM_VELOCITY | CLIENT_RW , SIM_POSITION | CLIENT_RW });
  Joint[_RF][Wheel] = CoppeliaSim->Add_Object("RF", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RW });
  Joint[_RB][Wheel] = CoppeliaSim->Add_Object("RB", JOINT, { SIM_VELOCITY | CLIENT_RW , SIM_POSITION | CLIENT_RW });
  Joint[_LB][Wheel] = CoppeliaSim->Add_Object("LB", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RW });
  Joint[_LM][Wheel] = CoppeliaSim->Add_Object("LM", JOINT, { SIM_VELOCITY | CLIENT_RW , SIM_POSITION | CLIENT_RW });
  Joint[_RM][Wheel] = CoppeliaSim->Add_Object("RM", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RW });

 // Angle[0]= CoppeliaSim->Add_Object("AngX", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
 // Angle[1] = CoppeliaSim->Add_Object("AngY", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
//  Angle[2] = CoppeliaSim->Add_Object("AngZ", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });

  /* Applications Init ----------------*/
 // User_Tasks_Init();
//  Task_RemoteControl_Init();
//  Service_Engineer_Init();
  /* Start the tasks and timer running. */
 // vTaskStartScheduler();
}


/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/

