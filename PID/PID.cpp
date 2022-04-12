/*
***********************************************************************
*					     ___ _____   _____ __  __ _____               *
*					  / ____|  __ \ / ____|  \/  |  __ \              *
*					 | |    | |  | | |  __| \  / | |  | |             *
*					 | |    | |  | | | |_ | |\/| | |  | |             *
*					 | |____| |__| | |__| | |  | | |__| |             *
*					  \_____|_____/ \_____|_|  |_|_____/              *
*					                                                  *
***********************************************************************				                                     
*
*  Useful PID Library
*
*  Copyright © 2022 Rafael de Bie
*  Permission is hereby granted, free of charge, to any person obtaining a
*  copy of this software and associated documentation files (the "Software"),
*  to deal in the Software without restriction, including without limitation
*  the rights to use, copy, modify, merge, publish, distribute, sublicense,
*  and/or sell copies of the Software, and to permit persons to whom the
*  Software is furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in all copies or 
*  substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*************************************************************************
*
* The API for this library:
*
* PID(float Kp, float Ki, float Kd): Constructor. Initializes Kp, Ki and Kd constants.
*	-> Kp: the Kp constant
*	-> Ki: the Ki constant
*	-> Kd: the Kd constant
*
* setValues(float Kp, float Ki, float Kd): Redefines Kp, Ki, and Kd constants.
*	-> Kp: the new Kp constant
*	-> Ki: the new Ki constant
*	-> Kd: the new Kd constant
*
* calculate(float target, float variable): calculates the PID output, given the target and variable.
*	-> the target variable of the PID
*	-> the variable to be processed
*
************************************************************************
*/






#include "Arduino.h"
#include "PID.h"
#include "C:\Program Files (x86)\Arduino\libraries\LiquidCrystal\src\LiquidCrystal.h"
 
using namespace std;


/// The constructor of the PID class
/// Sets Kp, Ki and Kd
///
PID::PID(float Kp, float Ki, float Kd)
{
	setValues(Kp,Ki,Kd);
}

/// sets new values for Kp, Ki and Kd
/// 
///
void PID::setValues(float Kp, float Ki, float Kd)
{
	_Kp = Kp;
	_Ki = Ki;
	_Kd = Kd;
}

/// Calculates the value for the PID.
/// Also updates Kd values
///
float PID::calculate(float target, float variable)
{
	float delta = target - variable;
	float value = 0;
	
	value += delta * _Kp;
	value += total * _Ki;
	value += (delta - last) * _Kd;
	total += delta;
	last = delta;
	return value;
}