#pragma once

#ifdef __cplusplus

class easy_pid
{
public:
    easy_pid() {}
    easy_pid(float _Kp, float _Ki, float _Kd) : Kp(_Kp), Ki(_Ki), Kd(_Kd) {}
    void SetPIDParam(float _Kp, float _Ki, float _Kd, float _I_Term_Max, float _Out_Max)
    {
        Kp = _Kp;
        Ki = _Ki;
        Kd = _Kd;
        I_Term_Max = _I_Term_Max;
        Out_Max = _Out_Max;
    };
    float Adjust();
    float Target = 0, Current = 0, Error;
    float Out = 0;

    float Kp = 0, Ki = 0, Kd = 0;
    float I_Term_Max = 0;        /*<! IÏîÏÞ·ù */
    float Out_Max = 0;

    float DeadZone = 0;

private:
    float pre_error = 0;
    float integral_e = 0;

    float pre_Current = 0;
    float I_Term = 0;
    float P_Term = 0;
    float D_Term = 0;
};





#endif