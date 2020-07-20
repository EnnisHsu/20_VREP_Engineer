#include "CoppeliaSim.h"
#include <string.h>
//TODO: Parameter checking.


/**
* @brief  Config object param.
*/
CoppeliaSim_Client::CoppeliaSim_Client()
{
  /*Init object list*/

}

/**
* @brief  Start simulation in specificed mode.
* @param  p_IP:               the ip address where the server is located.
* @param  p_conection_port:   the port number where to connect.
*                             Specify a negative port number in order to use shared
*                             memory, instead of socket communication.
* @param  commThreadCycleInMs:indicates how often data packets are sent back and forth.
*                             A default value of 5 is recommended
* @param  operationMode:Recommended operation mode for this function is simx_opmode_oneshot.
*                       To view more "Operation modes" please visit:
*                       http://www.coppeliarobotics.com/helpFiles/en/remoteApiConstants.htm#operationModes
* @retval true:         start simulation successfully.
* @retval false:        start failed.
*/
bool CoppeliaSim_Client::Start(const char *p_IP, int32_t p_conection_port, int commThreadCycleInMs,int operationMode)
{
  if (state == Connect_Lost)
  {
    /* Initialize simulation */
    clientID = simxStart(p_IP, p_conection_port, true, true, 2000, commThreadCycleInMs);
    
    if (clientID != -1)
    {
      simxStartSimulation(clientID, simx_opmode_oneshot);
      state = Connected;
      return true;
    }
    else
    {
      state = Connect_Lost;
      return false;
    }
  }
  else
    return true;
}

/**
* @brief  Functions to add objects(Joint, Vision sensor, Force sensor, Other Object in coppeliasim, Siganl). 
*         What object means is different from coppeliasim in this module. The "object" is an abstract
*         structure of data in programming.
* @note   We don't check the parameters in this version, including whether the object name is repeated, 
*         whether the operation list is valid and so on.
* @param  
* @return _simObjectHandle_Type* \ _simSignalHandle_Type*
* @author Mentos Seetoo
*/
_simObjectHandle_Type* CoppeliaSim_Client::Add_Object(std::string full_name, _simObj_Type type, std::initializer_list<simxInt> operation_ls)
{
  _simObjectHandle_Type* p_Obj = new _simObjectHandle_Type;

  /*Check the memory*/
  if (p_Obj == NULL) return p_Obj;
  
  /*Get object handle*/
  if (simxGetObjectHandle(clientID, full_name.data(), &p_Obj->obj_handle, simx_opmode_blocking) == simx_return_ok)
  {
    p_Obj->obj_name = full_name;

    /*Set the type.*/
    p_Obj->object_type = type;
    switch (type)
    {
      case JOINT:
        Joint_list.push_back(p_Obj);
        break;
      case FORCE_SENSOR:
        ForceSensor_list.push_back(p_Obj);
        break;
      case OTHER_OBJECT:
        OtherObject_list.push_back(p_Obj);
        break;
      case VISION_SENSOR:
        VisionSensor_list.push_back(p_Obj);
        break;
    }
    /*Set the operations.*/
    for each(auto operation in operation_ls)
    {
      _simOPParam_Struct* p_OP = new _simOPParam_Struct{0};
      p_OP->operation = operation;
      p_Obj->operation_list.push_back(*p_OP);
      delete p_OP;
    }
  }
  else
  { 
    /*Failed to get the object handle */
    delete p_Obj;
    p_Obj = NULL;
  }

  return p_Obj;
}

_simSignalHandle_Type* CoppeliaSim_Client::Add_Object(std::string full_name, _simSignal_Type type, std::initializer_list<simxInt> operation_ls)
{
  _simSignalHandle_Type* p_Signal = new _simSignalHandle_Type;

  /*Check the memory*/
  if (p_Signal == NULL) return p_Signal;

  p_Signal->siganl_name = full_name;
  p_Signal->signal_type = type;
  Signal_list.push_back(p_Signal);
  /*Set the operations.*/
  for each (auto operation in operation_ls)
  {
    _simOPParam_Struct* p_OP = new _simOPParam_Struct{ 0 };
    p_OP->operation = operation;
    p_Signal->operation_list.push_back(*p_OP);
    delete p_OP;
  }

  return p_Signal;
}

/**
* @brief  Functions to delete objects.
* @param  pObject: pointer of the object that will be deleted
* @return 1:operated.
*         0:aborted.
* @author Mentos Seetoo
*/
bool CoppeliaSim_Client::Delete_Object(_simObjectHandle_Type* pObject)
{
  /*Temporarily not do anything*/
  return false;
}

_clientStatus_Type CoppeliaSim_Client::Is_Connected()
{
  return state;
}

bool CoppeliaSim_Client::ComWithServer()
{
  /* Communication is established */
  if (clientID != -1)
  {
    /*Iterate all Joints*/
    for each (auto Joint in Joint_list)
    {
      for each (auto OP_Obj in Joint->operation_list)
      {
        /*Read operation*/
        if (GetIO_Type(OP_Obj.operation) != CLIENT_WO)
        {
          switch (GetOP_Type(OP_Obj.operation))
          {
            case SIM_POSITION:
              OP_Obj.read_return_code = simxGetJointPosition(clientID, Joint->obj_handle, &Joint->obj_Data.angle_f, simx_opmode_streaming);
              break;
            case SIM_FORCE:
              OP_Obj.read_return_code = simxGetJointForce(clientID, Joint->obj_handle, &Joint->obj_Data.force_3f[2], simx_opmode_streaming);
              break;
            default:
              break;
          }
        }
        /*Write operation*/
        if (GetIO_Type(OP_Obj.operation) != CLIENT_RO)
        {
          switch (GetOP_Type(OP_Obj.operation))
          {
            case SIM_POSITION:
              OP_Obj.write_return_code = simxSetJointTargetPosition(clientID, Joint->obj_handle, Joint->obj_Target.angle_f, simx_opmode_oneshot);
              break;
            case SIM_VELOCITY:
              OP_Obj.write_return_code = simxSetJointTargetVelocity(clientID, Joint->obj_handle, Joint->obj_Target.angVelocity_f, simx_opmode_oneshot);
              break;
            default:
              break;
          }
        }

        if ((OP_Obj.read_return_code | OP_Obj.write_return_code & 0x02) == 1)
        {
          /*Do nothing*/
        }
      }
    }

    /*Iterate all other_objects*/
    for each (auto OtherObject in OtherObject_list)
    {
      for each (auto OP_Obj in OtherObject->operation_list)
      {
        /*Read operation*/
        if (GetIO_Type(OP_Obj.operation) != CLIENT_WO)
        {
          switch (GetOP_Type(OP_Obj.operation))
          {
            case SIM_POSITION:
              /*Absolute position*/
              OP_Obj.read_return_code = simxGetObjectPosition(clientID, OtherObject->obj_handle, -1, OtherObject->obj_Data.position_3f, simx_opmode_streaming);
              break;
            case SIM_VELOCITY:
              OP_Obj.read_return_code = simxGetObjectVelocity(clientID, OtherObject->obj_handle, OtherObject->obj_Data.linVelocity_3f, OtherObject->obj_Data.angVelocity_3f, simx_opmode_streaming);
              break;
            case SIM_QUATERNION:
              /*Absolute quaternion*/
              OP_Obj.read_return_code = simxGetObjectQuaternion(clientID, OtherObject->obj_handle, -1, OtherObject->obj_Data.quaternion_4f, simx_opmode_streaming);
              break;
            case SIM_ORIENTATION:
              OP_Obj.read_return_code = simxGetObjectOrientation(clientID, OtherObject->obj_handle, -1, OtherObject->obj_Data.orientation_3f, simx_opmode_streaming);
              break;
            default:
              break;
          }
        }
        /*Write operation*/
        if (GetIO_Type(OP_Obj.operation) != CLIENT_RO)
        {
          switch (GetOP_Type(OP_Obj.operation))
          {
            case SIM_POSITION:
              /*Absolute position*/
              OP_Obj.write_return_code = simxSetObjectPosition(clientID, OtherObject->obj_handle, -1, OtherObject->obj_Target.position_3f, simx_opmode_oneshot);
              break;
            case SIM_QUATERNION:
              /*Absolute quaternion*/
              OP_Obj.write_return_code = simxSetObjectOrientation(clientID, OtherObject->obj_handle, -1, OtherObject->obj_Target.quaternion_4f, simx_opmode_oneshot);
              break;
            case SIM_ORIENTATION:
              OP_Obj.write_return_code = simxSetObjectOrientation(clientID, OtherObject->obj_handle, -1, OtherObject->obj_Target.orientation_3f, simx_opmode_oneshot);
              break;
            default:
              break;
          }
        }

        if ((OP_Obj.read_return_code | OP_Obj.write_return_code & 0x02) == 1)
        {
          /*Do nothing*/
        }
      }
    }

    /*Iterate all vision sensors*/
    for each (auto VisionSensor in VisionSensor_list)
    {
      for each (auto OP_Obj in VisionSensor->operation_list)
      {
        /*Read operation*/
        if (GetIO_Type(OP_Obj.operation) == CLIENT_RO)
        {
          switch (GetOP_Type(OP_Obj.operation))
          {
            case SIM_NORMAL_IMAGE:
                OP_Obj.read_return_code = simxGetVisionSensorImage(clientID, VisionSensor->obj_handle, VisionSensor->resolution, &VisionSensor->image_buffer, 0, simx_opmode_oneshot);
              break;
            case SIM_DEPTH_IMAGE:
              OP_Obj.read_return_code = simxGetVisionSensorDepthBuffer(clientID, VisionSensor->obj_handle, VisionSensor->resolution, &VisionSensor->depth_buffer, simx_opmode_oneshot);
              break;
            default:
              break;
          }
        }
        if ((OP_Obj.read_return_code | OP_Obj.write_return_code & 0x02) == 1)
        {
          /*Do nothing*/
        }
      }
    }
    /*Iterate all force sensors*/
    for each (auto ForceSensor in ForceSensor_list)
    {
      for each (auto OP_Obj in ForceSensor->operation_list)
      {
        /*Read operation*/
        if (GetIO_Type(OP_Obj.operation) == CLIENT_RO)
        {
          switch (GetOP_Type(OP_Obj.operation))
          {
            case SIM_FORCE:
              OP_Obj.read_return_code = simxReadForceSensor(clientID, ForceSensor->obj_handle, NULL, ForceSensor->obj_Data.force_3f, ForceSensor->obj_Data.torque_3f, simx_opmode_streaming);
              break;
            default:
              break;
          }
        }

        if ((OP_Obj.read_return_code | OP_Obj.write_return_code & 0x02) == 1)
        {
          /*Do nothing*/
        }
      }
    }
    /*Iterate all signals*/
    for each (auto Signal in Signal_list)
    {
      for each (auto OP_Obj in Signal->operation_list)
      {
        /*Check the operation type*/
        if (GetOP_Type(OP_Obj.operation) == SIM_SIGNAL_OP)
        {
          /*Read operation*/
          if (GetIO_Type(OP_Obj.operation) != CLIENT_WO)
          {
            simxInt temp = 0;
            /*Check the data type*/
            switch (Signal->signal_type)
            {
              case SIM_FLOAT_SIGNAL:
                OP_Obj.read_return_code = simxGetFloatSignal(clientID, Signal->siganl_name.data(), &Signal->data, simx_opmode_streaming);
                break;
              case SIM_STRING_SIGNAL:
                OP_Obj.read_return_code = simxGetStringSignal(clientID, Signal->siganl_name.data(), &Signal->string_data, &Signal->string_len, simx_opmode_streaming);
                break;
              case SIM_INTEGER_SIGNAL:
                OP_Obj.read_return_code = simxGetIntegerSignal(clientID, Signal->siganl_name.data(), &temp, simx_opmode_streaming);
                Signal->data = (simxFloat)temp;
                break;
              default:
                break;
            }
          }
          /*Write operation*/
          if (GetIO_Type(OP_Obj.operation) != CLIENT_RO)
          {
            /*Check the data type*/
            switch (Signal->signal_type)
            {
            case SIM_FLOAT_SIGNAL:
              OP_Obj.write_return_code = simxSetFloatSignal(clientID, Signal->siganl_name.data(), Signal->target, simx_opmode_oneshot);
              break;
            case SIM_STRING_SIGNAL:
              OP_Obj.write_return_code  = simxSetStringSignal(clientID, Signal->siganl_name.data(), Signal->string_data, Signal->string_len, simx_opmode_oneshot);
              break;
            case SIM_INTEGER_SIGNAL:
              OP_Obj.write_return_code = simxSetIntegerSignal(clientID, Signal->siganl_name.data(), (simxInt)Signal->target , simx_opmode_oneshot);
              break;
            default:
              break;
            }
          }
          if ((OP_Obj.read_return_code | OP_Obj.write_return_code & 0x02) == 1)
          {
            /*Do nothing*/
          }
        }
      }
    }
    return true;
  }
  else return false;
}

/**
* @brief  Stop simulation and clean up.
* @param  None.
* @retval None.
*/
CoppeliaSim_Client::~CoppeliaSim_Client()
{
  /*Clean up memory*/
  for each (auto Joint in Joint_list)
    delete Joint;
  Joint_list.clear();
  std::vector<_simObjectHandle_Type*>().swap(Joint_list);

  for each (auto ForceSensor in ForceSensor_list)
    delete ForceSensor;
  ForceSensor_list.clear();
  std::vector<_simObjectHandle_Type*>().swap(ForceSensor_list);

  for each (auto OtherObject in OtherObject_list)
    delete OtherObject;
  OtherObject_list.clear();
  std::vector<_simObjectHandle_Type*>().swap(OtherObject_list);

  for each (auto VisionSensor in VisionSensor_list)
    delete VisionSensor;
  VisionSensor_list.clear();
  std::vector<_simObjectHandle_Type*>().swap(VisionSensor_list);

  for each (auto Signal in Signal_list)
    delete Signal;
  Signal_list.clear();
  std::vector<_simSignalHandle_Type*>().swap(Signal_list);

  /* End simulation service */
  simxStopSimulation(clientID, simx_opmode_oneshot);
  simxFinish(clientID);
}