#pragma once
#include "extApi.h"
#include <string>
#include <vector>
#include <initializer_list>
/* Marco Definition --------------------------------------- */

/* Type Definition ---------------------------------------- */
enum _clientStatus_Type
{
  Connect_Lost = 0U,
  Connected
};

 
enum _simObj_Type
{
  JOINT = 0,
  OTHER_OBJECT,
  VISION_SENSOR,
  FORCE_SENSOR,
};

enum _simSignal_Type
{
  SIM_FLOAT_SIGNAL,
  SIM_INTEGER_SIGNAL,
  SIM_STRING_SIGNAL
};

enum _simIO_Type
{
  CLIENT_RO = 1 << 4, /*Read only*/
  CLIENT_WO = 2 << 4, /*Write only*/
  CLIENT_RW = 3 << 4  /*Read and Write*/
};

enum _simOP_Type
{
  SIM_POSITION = 0,
  SIM_QUATERNION,
  SIM_ORIENTATION,
  SIM_VELOCITY,
  SIM_FORCE,
  SIM_NORMAL_IMAGE,
  SIM_DEPTH_IMAGE,
  SIM_SIGNAL_OP       /*For signal operation*/
};

/* Structure Definition ----------------------------------- */
typedef struct
{
  simxInt operation;
  simxInt read_return_code;
  simxInt write_return_code;
}_simOPParam_Struct;

typedef struct
{
  simxFloat quaternion_4f[4],
    position_3f[3],
    orientation_3f[3],
    angVelocity_3f[3], //angle velocity(3-dimension)
    linVelocity_3f[3], //linear velocity(3-dimension)
    force_3f[3],       //force from force sensor(3-dimension)
    torque_3f[3],      //torque of joint (3-dimension),when reading a Joint, it only use Z-Axis(torque_3f[2])
    angle_f,           //angle of joint
    angVelocity_f;     //angular velocity of joint
}_simOPData_Struct;

typedef struct
{
  std::string  obj_name;
  simxInt      obj_handle;
  _simObj_Type object_type;
  std::vector<_simOPParam_Struct>  operation_list;      //operation = _simOP_Type || _simIO_Type;
  _simOPData_Struct obj_Data = { 0 };                   //Data to read
  _simOPData_Struct obj_Target = { 0 };                 //Taeget to write
  simxUChar*   image_buffer;                            //Normal image buffer of vision sensor
  simxFloat*   depth_buffer;                            //Depth image buffer of vision sensor
  simxInt      resolution[2];                           //Resolution of vision sensor
}_simObjectHandle_Type;

typedef struct 
{
  std::string     siganl_name;
  _simSignal_Type signal_type;
  std::vector<_simOPParam_Struct>  operation_list;      //operation = _simOP_Type || _simIO_Type;
  simxFloat data;
  simxFloat target;
  simxUChar* string_data;
  simxInt    string_len;
}_simSignalHandle_Type;

class CoppeliaSim_Client
{
public:
  CoppeliaSim_Client();
  ~CoppeliaSim_Client();

  bool Start(const char *p_IP, int32_t p_conection_port, int commThreadCycleInMs, int operationMode);
  _simObjectHandle_Type* Add_Object(std::string full_name, _simObj_Type type, std::initializer_list<simxInt> operation_ls);
  _simSignalHandle_Type* Add_Object(std::string full_name, _simSignal_Type type, std::initializer_list<simxInt> operation_ls);
  bool Delete_Object(_simObjectHandle_Type* pObject);
  bool ComWithServer();
  _clientStatus_Type Is_Connected();
  int32_t clientID;
private:
 

  std::vector<_simObjectHandle_Type*>  Joint_list;
  std::vector<_simObjectHandle_Type*>  ForceSensor_list;
  std::vector<_simObjectHandle_Type*>  OtherObject_list;
  std::vector<_simObjectHandle_Type*>  VisionSensor_list;
  std::vector<_simSignalHandle_Type*>  Signal_list;

  _clientStatus_Type state;

  _simOP_Type GetOP_Type(simxInt operation) {
    return (_simOP_Type)(operation & 0x0F);
  }

  _simIO_Type GetIO_Type(simxInt operation) {
    return (_simIO_Type)(operation & 0xF0);
  }
};