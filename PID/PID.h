#ifndef PID_h
#define PID_h


#include <inttypes.h>
#include "Arduino.h"

class PID
{
	public:
	
	PID(float Kp, float Ki, float Kd);									//PID constructor
	void setValues(float Kp, float Ki, float Kd);						//Updates the Kp, Ki, and Kd values of the PID
	float calculate(float target, float variable);						//Calculates and updates the PID
	
	float _Kp = 0;														//The Kp Constant 
	float _Ki = 0;														//The Ki Constant
	float _Kd = 0;														//The Kd Constant
	
	private:
	float last = 0;														//The last delta value (Used for Kd)
	float total = 0;													//The total delta value (Used for Ki)
	
};



#endif