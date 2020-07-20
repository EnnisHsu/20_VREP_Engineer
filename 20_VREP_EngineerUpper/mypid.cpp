#include "mypid.h"

template<typename Type>
Type _pid_Abs(Type x) { return ((x > 0) ? x : -x); }

template<typename Type>
void _pid_Constrain(Type* x, Type Min, Type Max)
{
    if (*x < Min) *x = Min;
    else if (*x > Max) *x = Max;
    else { ; }
}

float easy_pid::Adjust()
{
    /*Error time*/
    static int index=0;
    if (Target == 0)
    {
        index++;
    }
    else 
    {
        index = 0;
    }
    if (index == 3)
    {
        index = 0;
        Out = 0; 
        return Out;
    }
    Error = Target - Current;

    if (_pid_Abs(Error) < DeadZone)
    {
        Out = 0;
        return Out;
    }

    /* Using Low-Pass Filter to preprocess*/

    P_Term = Kp * Error;

    /* Using Integral separation */
    integral_e += Error;
        /* PID with Changing integration rate */

     I_Term = Ki * integral_e;
        /*Constarin*/
     _pid_Constrain(&integral_e, -I_Term_Max, I_Term_Max);

    float d_err = 0;
    d_err = Error - pre_error;
    D_Term = Kd * d_err;

    pre_error = Error;

    Out = P_Term + I_Term + D_Term;

    /* Constarin */
    _pid_Constrain(&Out, -Out_Max, Out_Max);

    return Out;
}