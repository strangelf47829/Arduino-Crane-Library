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
*  Zuyd Crane Project
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
*
*
* Crane(uint8_t arduinoID): Crane Constructor
* 	-> arduinoID: the ID of the arduino (1,2,3)
* 
* verify(): Calls the arduinos respective verify function
* 
* update(): Calls the arduinos respective update function
* 
* init():   Calls the arduinos respective init function
* 
* returnHC06Msg(): Checks for any message from the HC-06. Returns the incoming data as a string, if any
* 
* onReceive(): Automatically parses some basic I2C commands. It should be the first thing called in the implementation. Returns the incoming data as a string
* 
* subscribe(uint8_t index): Subscribes an index. Used to flag indicies which must be sent at a later time.
* 	-> index: The index to be subscribed
* 
* sendData(uint8_t arduino, byte data): Sends a byte of data to arduino (read: address) 'arduino'.
* 	-> arduino: the address of the arduino.
* 	-> data: the byte to be sent
* 	
* sendData(uint8_t arduino, String data): Sends a string (as a sequence of bytes) to arduino (read: address) 'arduino'.
* 	-> arduino: the address of the arduino.
* 	-> data: the string to be sent
* 
* removeNFromBuffer(uint8_t n): removes 'n' amount of entries from the instruction buffer
* 	-> n: the amount of instructions to remove
* 	
* readBuffer(): reads the latest entry in the buffer, returns it, and removes it from the buffer
* 
* available(): returns the amount of unread buffer entries
* 
* flushBuffer(): clears the buffer.
* 
* addToBuffer(String instruction): adds the instruction to the buffer
* 	-> instruction: the instruction to be added
* 
* pushBuffer(uint8_t ardId, uint8_t n): Pushes (sends) subscribed indicies to arduino (read: address). Only checks 'n' indexes
* 	-> ardId: the arduino to push to
* 	-> n: the amount of indicies to send (if subscribed)
* 
* pushBuffer(uint8_t ardId): Pushes (sends) all subscribed indicies to arduino (read: address).
* 	-> ardId: the arduino to push to
*
*
************************
*
*
* init1(): initializes the arduino. called externally. Dont call directly, call "verify()" instead
*
* update1(): Excecutes repetitive tasks. called externally. Dont call directly, call "update()" instead
*
* verify1(): checks the integrity of the construction. called externally. Dont call directly, call "verify()" instead
*
*
************************
*
*
* init2(): initializes the arduino. called externally. Dont call directly, call "verify()" instead
*
* update2(): Excecutes repetitive tasks. called externally. Dont call directly, call "update()" instead
*
* verify2(): checks the integrity of the construction. called externally. Dont call directly, call "verify()" instead
* 
* setSpeedOf(uint8_t stepper, float rps): Sets the speed of stepper 'stepper' to 'rps' (in rotations per second). Returns 0
* 	-> stepper: the ID of the stepper (1 -> stepper 1). 
*	-> rps: the speed of the stepper motor in rotations per second. Note: the sign of this parameter determines the direction of the movement 
* 
* LCM(float n1, float n2, float n3): Returns the LCM of three numbers (rounded up).
* 	-> n1: an arbitrary number
* 	-> n2: an arbitrary number
*	-> n3: an arbitrary number
*
* maX(uint8_t a, uint8_t b): Returns a if a > b. otherwise returns b
*	-> a: an arbitrary number
*	-> b: an arbitrary number
*
* stepSync(): Steps all motors in a synchronized manner
*
* step(uint8_t stepper): Steps the specified stepper once
* 	-> stepper: the stepper id
*
* step(uint8_t stepper, bool direction): Steps the specified stepper once in a specified direction
* 	-> stepper: the stepper id
*	-> direction: the direction the stepper step
*
*
************************
*
*
* init3(): initializes the arduino. called externally. Dont call directly, call "verify()" instead
*
* update3(): Excecutes repetitive tasks. called externally. Dont call directly, call "update()" instead
*
* verify3(): checks the integrity of the construction. called externally. Dont call directly, call "verify()" instead
*
* createCustomChars(): Creates the custom characters for the LCD screen


************************************************************************
*/






#include "Arduino.h"
#include "Crane.h"
#include "C:\Program Files (x86)\Arduino\libraries\LiquidCrystal\src\LiquidCrystal.h"
#include <inttypes.h>
#include <stdint.h>
#include "Wire.h"
#include "LiquidCrystal.h"
#include "Stepper.h"
#include "SoftwareSerial.h"
 
using namespace std;

/// The constructor of the crane class
/// Doesnt do much, because the 'Constructing' happens at the 'init()'.
///
Crane::Crane(uint8_t arduinoID)
{
	//-------------------------------- Set own arduinoID to the parameter
	_arduinoID = arduinoID;
}

/// This function initializes the board, to make it ready for operations
/// More specifically, the generic "init()" function calls "init1()", "init2()", "init3()" respective of the arduino ID
///
int Crane::verify()
{
	//-------------------------------- Divert the 'verify()' to the arduinos specific "verify()" functions
	if(devMode) Serial.println("------------- Verifying board");
	if(_arduinoID == 1) return verify1();
	if(_arduinoID == 2) return verify2();
	if(_arduinoID == 3) return verify3();
}

/// This function initializes the board, to make it ready for operations
/// More specifically, the generic "init()" function calls "init1()", "init2()", "init3()" respective of the arduino ID
///
void Crane::update()
{
	//-------------------------------- Divert the 'update()' to the arduinos specific "update()" functions
	if(_arduinoID == 1) update1();
	if(_arduinoID == 2) update2();
	if(_arduinoID == 3) update3();
}

/// This function initializes the board, to make it ready for operations
/// More specifically, the generic "init()" function calls "init1()", "init2()", "init3()" respective of the arduino ID
///
int Crane::init()
{
	//-------------------------------- Divert the 'init()' to the arduinos specific "init()" functions
	if(devMode) Serial.println("------------- Initializing board");
	if(_arduinoID == 1) init1();
	if(_arduinoID == 2) init2();
	if(_arduinoID == 3) init3();
}

/// Read the HC-06 buffer, if there is anything at all
/// Returns the incoming HC-06 message as a string (data read as char)
///
String Crane::returnHC06Msg()
{
	//-------------------------------- Declare return string
	String ret = "";
	
	//-------------------------------- Is there anything in the buffer?
	if (hcSerial.available()) {
	
	//-------------------------------- Read anything until the buffer is empty
		while(hcSerial.available()) 
		{ 
	//-------------------------------- Add the incoming byte as a char to the return string
			ret += (char)hcSerial.read();
		}   
	  Serial.println("Blo tand: " + ret);
	}
	
	//-------------------------------- Return the return string
	return ret;
}

/// This function is called everytime something is received over the I2C bus (it must be the first thing called in the implementation)
/// It returns a string of the received data (as an array of chars)
/// 
String Crane::onReceive(int bytes)
{
	
	if(devMode) Serial.print("Receiving Something: ");
	
	//-------------------------------- Declare return string
	String in = "";
	
	//-------------------------------- Add all incoming bytes onto the return string (as char)
	while(Wire.available())
	{
		in += (char)Wire.read();
	}
	
	if(devMode) Serial.println(in);
	
	//-------------------------------- If the incoming message is a ping call, add "OK2" (Arduino 2) or "OK3" (Arduino 3) to the instruction buffer and subscribe the message
	if(in == "Ping")
	{ if(_arduinoID == 2) { addToBuffer("OK2"); subscribe(bufferLength -1); } else { addToBuffer("OK3"); subscribe(bufferLength -1);} }
	
	//-------------------------------- If the incoming message is "OK2" or "OK3", then set the respective verified flag to true.
	else if(in == "OK2") { arduino2Verify = true; digitalWrite(10,HIGH);} else if(in == "OK3") { arduino3Verify = true; arduino2Verify = true; digitalWrite(11,HIGH);}
	
	//-------------------------------- If the incoming message is "VerifyOK", the board has been verified successfully.
	else if(in == "VerifyOK") boardVerified = true;
	
	//-------------------------------- If the incoming message is "CONNECTED", set the blueConnected flag to true
	if(in == "CONNECTED")	blueConnected = true;
	
	//-------------------------------- If the incoming message is "blueOK", set blueState to 1
	if(in == "blueOK")
		blueState = 1;
	
	//-------------------------------- If the incoming message is "blueERR", set blueState to 2
	else if(in == "blueERR")
		blueState = 2;
	
	//-------------------------------- if the incoming message is "blueINOP", set blueState to 3
	else if(in == "blueINOP")
		blueState = 3;
	
	
	//-------------------------------- Return the incoming string for external processing
	return in; 
	
}

/// Subscribes an index
/// This is used to flag indexes that have to be sent at a later time
///
void Crane::subscribe(uint8_t index)
{
	//-------------------------------- Subscribe the index: Set the value at index 'index' to true
	subscribed[index] = true;
	if(devMode) { Serial.print("Subscribing index "); Serial.println(index); }
}

/// Sends a byte of data over the I2C bus
/// 
///
void Crane::sendData(uint8_t arduino, byte data)
{
	if(devMode) { Serial.print("Sending \""); Serial.print(data); Serial.print("\" To arduino "); Serial.println(arduino); }
	
	//-------------------------------- Begin transmission to address 'arduino', and send the data
	Wire.beginTransmission(arduino);
	Wire.write(data);
	Wire.endTransmission(); 
}

/// Sends a string over the I2C bus
/// This is done by sequentially sending each byte in the string
///
void Crane::sendData(uint8_t arduino, String data)
{
	if(devMode) { Serial.print("Sending \""); Serial.print(data); Serial.print("\" To arduino "); Serial.println(arduino); }
	
	//-------------------------------- Begin transmission to address 'arduino', and sequentially send the data in 'data'
	Wire.beginTransmission(arduino);
	for(byte b : data)
	{
		Wire.write(b);
	}
	Wire.endTransmission();
	
}

/// Removes 'n' amount of indexes from the instruction buffer
/// Note: the data is still 'available', its just treated as if it does not exist
///
void Crane::removeNFromBuffer(uint8_t n)
{
	//-------------------------------- artificially reduce the amount of elements in the bufferLength and bufferIndex
	bufferLength -= n;
	bufferIndex -= n;
}

/// Return the latest entry in the buffer
/// Does not remove the entry, just makes it invisible.
///
String Crane::readBuffer()
{
	//-------------------------------- Declare return string
	String value = "";
	
	//-------------------------------- If the index is less than the length, the return value is the value at index 'bufferIndex', and increase it by 1.
	if(bufferIndex < bufferLength)
	{
		value = _instrBuffer[bufferIndex++];
		return value;
	}
	
	//-------------------------------- If the index is higher than the length of the buffer (No data available), return "EMPTY"
	else
		return "EMPTY";
}

/// Returns the amount of available entries
/// 
///
int Crane::available()
{
	//-------------------------------- If the buffer is overflowing, flush it.
	if(bufferLength == 35 && bufferIndex == 34)
		flushBuffer();
	
	//-------------------------------- If the index is equal to or higher than the length of the buffer, return 0
	if(bufferLength <= bufferIndex+1)
	{
		return 0;
	}
	
	//-------------------------------- If there is something available, return the difference between the length and the index
	return bufferLength - bufferIndex;
}

/// Clears the buffer of everything
/// 
///
void Crane::flushBuffer()
{
	bufferLength = 0;
	bufferIndex = 0;
}

/// Adds an entry to the buffer
/// 
///
void Crane::addToBuffer(String instruction)
{

	_instrBuffer[bufferLength++] = instruction;
	if(devMode)
	{ Serial.print("Adding \""); Serial.print(instruction); Serial.print("\" to the buffer at index "); Serial.print(bufferLength); Serial.print(". Current index is "); Serial.println(bufferIndex); }
		
}

/// pushes subscribed elements to I2C address "arId"
/// only checks for n amount of things on the buffer, not n amount of subscribed elements
///
void Crane::pushBuffer(uint8_t arId, uint8_t n)
{
	uint8_t x;
	for(x = 0; x < n; x++)
	{
		if(!subscribed[x]) sendData(arId, readBuffer());
	}
}

/// pushes all subscribed elements to I2C address "arId"
/// 
///
void Crane::pushBuffer(uint8_t ardId)
{
	for(int x = 0; x < 16; x++)
	{
		if(subscribed[x]) sendData(ardId, readBuffer());
		subscribed[x] = false;
	}
}






/* FUNCTIONS ARDUINO 1
*
* init1(): initializes the arduino. called externally. Dont call directly, call "verify()" instead
* update1(): Excecutes repetitive tasks. called externally. Dont call directly, call "update()" instead
* verify1(): checks the integrity of the construction. called externally. Dont call directly, call "verify()" instead
*
*/

/// The init function for arduino 1
/// Initializes the servo, sets pinModes, resets arduino 2
///
int Crane::init1()
{
	//-------------------------------- Set stepper relay high
	pinMode(9,OUTPUT);		
	digitalWrite(9,HIGH);
	
	//-------------------------------- Setting the pinModes
	pinMode(10,OUTPUT);
	pinMode(11,OUTPUT);
	digitalWrite(10,HIGH);
	digitalWrite(11,HIGH);
	
	pinMode(A6,OUTPUT);
	pinMode(6,INPUT);
	
	pinMode(7,OUTPUT);
	pinMode(8,INPUT);
	
	pinMode(A6,INPUT);
	
	pinMode(4,INPUT);
	pinMode(5,INPUT);
	
	pinMode(12,OUTPUT);
	
	//-------------------------------- Begin the wire communications on the I2C bus
	Wire.begin(1);
	
	//-------------------------------- Begin the Serial communications with the PC and HC06
	Serial.begin(9600);
	hcSerial.begin(9600);
	
	//-------------------------------- Send a "AT+VERSION" command to the HC06 
	hcSerial.write("AT+VERSION");
	blueState = -1;
	
	//-------------------------------- Reset speeds on arduino 2
	sendData(2,"STEP1:0.00");
	sendData(2,"STEP2:0.00");
	sendData(2,"STEP3:0.00");
	
	//--------------------------------- Attach the servo on pin 12
	grip.attach(12);
	

	
	return 1; //Return 1, Success (unused)
}

/// The verify function for arduino 1
/// pings arduino 2, arduino3, checks that the bluetooth module is valid.
///
int Crane::verify1()
{
	//-------------------------------- Resets the LEDS
	digitalWrite(10,LOW);
	digitalWrite(11,LOW);
	
	//-------------------------------- Pings arduino 2 and arduino 3
	sendData(3,"Ping");
	sendData(2,"Ping");
	Serial.println("Before");
	
	//-------------------------------- Check for a response from the bluetooth module
	String blueRespons = returnHC06Msg();
	Serial.println("Bluetooth module response: " + blueRespons);
	
	//-------------------------------- Send arduino 3 the status of the bluetooth module
	if(blueRespons == "OKlinvorV1.8" && blueState == -1)
		{ sendData(3,"blueOK"); blueState = 1; }		//Bluetooth module is ok
	
	else if(blueRespons != "" && blueState == -1)
		{ sendData(3,"blueERR"); blueState = 2; }		//Something was received, but not expected value. (Error)
	
	else if(blueRespons == "" && blueState == -1)
		{ sendData(3,"blueINOP"); blueState = 3; }		//Nothing was received at all. The communications failed.
	
	
	//-------------------------------- Wait for a ping response
	delay(7750);
	Serial.println("After");
	
	//-------------------------------- If one of the arduinos were not able to be verified, throw error.
	if(!arduino2Verify || !arduino3Verify)
	{ digitalWrite(10,LOW); digitalWrite(11,LOW); return -1;	}

	//-------------------------------- Else, if both of the arduinos were able to be verified, send an OK response.
	else if(arduino2Verify && arduino3Verify)
	{
		sendData(2,"VerifyOK"); sendData(3,"VerifyOK"); boardVerified = true;
	}
	
	//-------------------------------- Update the status leds (Green led: successfully verified, Red led: Verification failed)
	digitalWrite(10,LOW);
	digitalWrite(11,LOW);
	digitalWrite(boardVerified ? 10 : 11, HIGH);
	delay(5000);
	digitalWrite(boardVerified ? 10 : 11, LOW);
	
	grip.write(10);
	delay(1000);
	grip.write(160);
	delay(1000);
	grip.write(0);
	digitalWrite(10,HIGH);
}

/// The update function for arduino 1
/// gets the gripper height
///
void Crane::update1()
{
	//-------------------------------- Sends pulse to TRIG pin
	digitalWrite(A6,HIGH);
	delayMicroseconds(10);
	digitalWrite(A6,LOW);
	
	//-------------------------------- Parses return
	long duration = pulseIn(6,HIGH)  / 29 / 2;
	Serial.print("X:");
	Serial.print(duration);
	Serial.println("cm");
	
}




/* FUNCTIONS ARDUINO 2
*
* init2(): initializes the arduino. called externally. Dont call directly, call "verify()" instead
* update2(): Excecutes repetitive tasks. called externally. Dont call directly, call "update()" instead
* verify2(): checks the integrity of the construction. called externally. Dont call directly, call "verify()" instead
* 
* setSpeedOf(uint8_t stepper, float rps): Sets the speed of stepper 'stepper' to 'rps' (in rotations per second). Returns 0
* 	-> stepper: the ID of the stepper (1 -> stepper 1). 
*	-> rps: the speed of the stepper motor in rotations per second. Note: the sign of this parameter determines the direction of the movement 
* 
* LCM(float n1, float n2, float n3): Returns the LCM of three numbers (rounded up).
* 	-> n1: an arbitrary number
* 	-> n2: an arbitrary number
*	-> n3: an arbitrary number
*
* maX(uint8_t a, uint8_t b): Returns a if a > b. otherwise returns b
*	-> a: an arbitrary number
*	-> b: an arbitrary number
*
* stepSync(): Steps all motors in a synchronized manner
*
* step(uint8_t stepper): Steps the specified stepper once
* 	-> stepper: the stepper id
*
* step(uint8_t stepper, bool direction): Steps the specified stepper once in a specified direction
* 	-> stepper: the stepper id
*	-> direction: the direction the stepper step
*
*
*/

/// The init function for arduino 2
/// pinModes, enable stepper motors
///
int Crane::init2()
{
	//-------------------------------- Begin serial and wire
	Wire.begin(2);
	Serial.begin(9600);
	
	//-------------------------------- pinModes
	pinMode(2,OUTPUT); //Step 1
	pinMode(3,OUTPUT); //Dir 1
	pinMode(4,OUTPUT); //En 1 [AL]
	
	pinMode(5,OUTPUT); //Step 2
	pinMode(6,OUTPUT); //Dir 2
	pinMode(7,OUTPUT); //En 2 [AL]
	
	pinMode(8,OUTPUT); //Step 3
	pinMode(9,OUTPUT); //Dir 3
	pinMode(10,OUTPUT); //En 3 [AL]
	
	//-------------------------------- enable stepper drivers
	digitalWrite(4,LOW);
	digitalWrite(7,LOW);
	digitalWrite(10,LOW);
	
	
}

/// The verify function for arduino 2
/// Turns each stepper a certain speed
/// Note: This will move the crane. Be sure to allow for this movement
int Crane::verify2()
{
	//-------------------------------- Check each motor
	setSpeedOf(1,1);
	int mil = millis();
	
	//-------------------------------- Turn motor one both ways
	while(millis() - mil < 1000)
	{
		step(1,true);
		delayMicroseconds(250);
	}
	delay(500);
	setSpeedOf(1,2);
	mil = millis();
	while(millis() - mil < 1000)
	{
		step(1,false);
		delayMicroseconds(500);
	}
	
	delay(500);
	setSpeedOf(1,4);
	mil = millis();
	
	//-------------------------------- Turn motor two both ways
	while(millis() - mil < 1000)
	{
		step(2,true);
		delayMicroseconds(750);
	}
	delay(500);
	setSpeedOf(1,0.25);
	mil = millis();
	while(millis() - mil < 1000)
	{
		step(2,false);
		delayMicroseconds(1000);
	}
	
	delay(500);
	setSpeedOf(1,0.5);
	mil = millis();
	
	//-------------------------------- Turn motor three both ways
	while(millis() - mil < 1000)
	{
		step(3,true);
		delayMicroseconds(5000);
	}
	delay(500);
	mil = millis();
	while(millis() - mil < 1000)
	{
		step(3,false);
		delayMicroseconds(2500);
	}
	
	//-------------------------------- Resets the speeds of the motors
	setSpeedOf(1,0);
	setSpeedOf(2,0);
	setSpeedOf(3,0);
	
	//-------------------------------- Wait one second, then reply to ping message
	delay(1000);
	Serial.println("--Buff");
	pushBuffer(1);
}

/// The update function for arduino 2
/// Empty
/// 
void Crane::update2(){}

/// Sets the speed of each stepper motor
/// This allows for synchronous movement of the stepper motors
/// 
double Crane::setSpeedOf(uint8_t stepper, float rps)
{
	//-------------------------------- gets the absolute RPS
	float aRPS = rps < 0 ? -rps : rps;
	
	//-------------------------------- Sets the speed for the respective stepper
	switch(stepper)
	{
		case 1:
		if(aRPS > 0.02) delayStep1 = (5/aRPS); else delayStep1 = 250; //Calculate speeds
		digitalWrite(3, rps > 0); //Set direction pin high or low
		break;
		case 2:
		if(aRPS > 0.02) delayStep2 = (5/aRPS); else delayStep2 = 250; //Calculate speeds
		digitalWrite(6, rps > 0); //Set direction pin high or low
		break;
		case 3:
		if(aRPS > 0.02) delayStep3 = (5/aRPS); else delayStep3 = 250; //Calculate speeds
		digitalWrite(9, rps > 0); //Set direction pin high or low
		break;
	}
	//-------------------------------- Return 0. (WIP algorithm)
	return 0;
}

/// Finds the 'lowest*' common multiple of 3 numbers.
/// allows for synchronous movement of the stepper motors
/// (* it actually doesnt. its a lazily written algorithm....... )
int Crane::LCM(float n1, float n2, float n3)
{
	//-------------------------------- Declare the offset (unused)
	int offset = 0;
	
	//-------------------------------- If stepper 1 is disabled, act as if it steps every 10 milliseconds to ease calculations
	if(n1 == 250) { n1 = 10; offset = 0; }
	
	//-------------------------------- If stepper 2 is disabled, act as if it steps every 10 milliseconds to ease calculations
	if(n2 == 250) { n2 = 10; offset = 0; }
	
	//-------------------------------- If stepper 3 is disabled, act as if it steps every 10 milliseconds to ease calculations
	if(n3 == 250) { n3 = 10; offset = 0; }
	
	//-------------------------------- If the steppers each have the same step time, then return the value of stepper 1 (unless they are disabled)
	if(n1 == n2 && n2 == n3)
		if(n1 != 250) return ceil(n1); else return -1;
	
	//-------------------------------- If n1 == n2, then the LCM is just n1 * n3
	if(n1 == n2)
		return ceil(n1*n3) + offset;
	
	//-------------------------------- If n2 == n3, then the LCM is just n1 * n2
	if(n2 == n3)
		return ceil(n1*n2) + offset;
	
	//-------------------------------- If n1 == n3, then the LCM is just n1 * n2
	if(n1 == n3)
		return ceil(n1*n2) + offset;
	
	//-------------------------------- If none of the above apply, just bruteforce it
	for(float n = 1; n < 16; n++)
	{
		if(n1 * n == n2 * n && n2 * n == n3 * n)
		{
			return ceil(n1 * n) + offset;
		}
	}
	
	//-------------------------------- if not even the bruteforce method worked, just multiply all three
	return ceil(n1 * n2 * n3) + offset;
}

/// Returns a or b, whichever is highest
/// 
/// (For some reason, the standard max() function kept giving me compile errors, so i made this
uint8_t Crane::maX(uint8_t a, uint8_t b)
{
	return a > b ? a : b;
}

/// Steps the motors synchronously
/// This function must be called after eachother with little delay, or the movement will not be synchronous
/// ^ For this reason, do not use delays anywhere else in your code. (unless you are delaying microseconds)
void Crane::stepSync()
{
	//-------------------------------- calculate the LCM to ensure each stepper can step synchronously.
	runTime = LCM(maX(delayStep1,1),maX(delayStep2,1),maX(delayStep3,1));
	
	
	//-------------------------------- declare and initialize the lastx variables. This ensures at least one step.
	int lastx1 = -delayStep1, lastx2 = -delayStep2, lastx3 = -delayStep3;
	for(int x = 0; x < runTime; x++)
	{
		delay(1);
		if(x - lastx1 == delayStep1 && delayStep1 != 250) //If a predefined of milliseconds have passed,
		{step(1); lastx1 = x; }									//step motor one
		if(x - lastx2 == delayStep2 && delayStep2 != 250) //If a predefined of milliseconds have passed,
		{step(2); lastx2 = x; }									//step motor two
		if(x - lastx3 == delayStep3 && delayStep3 != 250) //If a predefined of milliseconds have passed,
		{step(3); lastx3 = x; }									//step motor three
		
	}
}

/// Step stepper 'stepper' once
/// The 'stepper' parameter specifies which stepper to drive (1,2,3)
/// 
void Crane::step(uint8_t stepper)
{
	//-------------------------------- Pulse the respective stepper step pin
	digitalWrite(2+(stepper-1)*3,HIGH);
	delayMicroseconds(3);
	digitalWrite(2+(stepper-1)*3,LOW);
}

/// Step stepper 'stepper' once, with direction 'direction'
/// 
/// 
void Crane::step(uint8_t stepper, bool direction)
{
	//-------------------------------- set the respective stepper dir pin
	digitalWrite(3+(stepper-1)*3,direction);
	
	//-------------------------------- Pulse the respective stepper step pin
	digitalWrite(2+(stepper-1)*3,HIGH);
	delayMicroseconds(3);
	digitalWrite(2+(stepper-1)*3,LOW);
}






/* FUNCTIONS ARDUINO 3
*
* init3(): initializes the arduino. called externally. Dont call directly, call "verify()" instead
* update3(): Excecutes repetitive tasks. called externally. Dont call directly, call "update()" instead
* verify3(): checks the integrity of the construction. called externally. Dont call directly, call "verify()" instead
*
* createCustomChars(): Creates the custom characters for the LCD screen
*
*/



/// The init function for arduino 3
/// pinModes, Display splash screen
///
int Crane::init3()
{
	//-------------------------------- pinModes
	pinMode(9,OUTPUT);
	pinMode(10,OUTPUT);
	pinMode(11,OUTPUT);
	
	//-------------------------------- initialize display
	_lcd.begin(16,2);
	_lcd.setCursor(0,0);
	
	//-------------------------------- verify the board
	verify();
	delay(2500);
	
	//-------------------------------- if not in dev mode, display company splash screen and death toll
	if(!devMode)
	{
		String name = "CDGMD I1             ";
		String comp = "CDGMD";
		
		//-------------------------------- Display company name
		_lcd.clear();
		_lcd.home();
		_lcd.print(comp);
		
		//-------------------------------- Scroll construction name to the left
		for(int x = 15; x != -1; x--)
		{
			_lcd.setCursor(0,1);
			_lcd.print("                ");
			_lcd.setCursor(x,1);
			_lcd.print(name);
			delay(250);
		}
		
		//-------------------------------- Scroll construction name to the left
		delay(2500);
		for(int x = 0; x < name.length(); x++)
		{
			
			_lcd.scrollDisplayLeft();
			delay(250);
		}
		delay(1000);
		
		//-------------------------------- display death toll
		_lcd.clear();
		_lcd.home();
		_lcd.write("In loving memory");
		_lcd.setCursor(0,1);
		_lcd.write("7 Arduinos");
		delay(1500);
		_lcd.setCursor(0,1);
		_lcd.write("5 Drivers       ");
		delay(1500);
		_lcd.setCursor(0,1);
		_lcd.write("Countless HC-06 ");
		delay(1500);
		
	}else
	{ _lcd.print("dev Mode"); delay(1500); }

	//-------------------------------- Clear everything. This is to reset everything with the scrolling.
	_lcd.begin(16,2);
	_lcd.setCursor(0,0);
	createCustomChars();
	
	
}

/// The update function for arduino 3
/// Display the UI of the crane.
/// Note: because the UI is on arduino 3, and the step functions are on arduino 2 and arduino 1 controls the step functions, ensure coordination of the UI elements between the arduinos
void Crane::update3()
{
	//-------------------------------- Clear the display
	_lcd.clear();
	_lcd.setCursor(14,0);
	
	//-------------------------------- If bluetooth connected, just display the connected graphic
	if(blueConnected)
	{
		_lcd.write(char(5));
		_lcd.write(char(7));
	}else
	{
	//-------------------------------- Just loop over the connection frames if bluetooth not connected
		frameConnect++;
		if(frameConnect == 4) frameConnect = 0;
		switch(frameConnect)
		{
			case 0: _lcd.write(char(4)); break;
			case 1: _lcd.write(char(5)); break;
			case 2: _lcd.write(char(5)); _lcd.write(char(6)); break;
			case 3: _lcd.write(char(5)); _lcd.write(char(7)); break;
		}
		digitalWrite(10,frameConnect == 0 || frameConnect == 2);
	}
	
	//-------------------------------- Set the status LED (the higher the condition, the higher priority it has
	
	//-------------------------------- Set led to yellow if the voltage is lower than 10.75 volts
	if(voltage < 10.75)
	{
		analogWrite(9,255);
		analogWrite(11,84);
		analogWrite(10,0);
	}
	
	//-------------------------------- If bluetooth was not connected, halt the if-else braiding. The status LED was set earlier
	else if(!blueConnected) {}
	
	//-------------------------------- If the crane is in standby, set LED to white
	else if(statusString == "Standby" || statusString == "Cooling")
	{
		digitalWrite(9,HIGH);
		digitalWrite(11,HIGH);
		digitalWrite(10,HIGH);
	}
	
	//-------------------------------- If the board was not verified, set LED to green
	else if(!boardVerified)
	{
		digitalWrite(9,LOW);
		digitalWrite(11,HIGH);
		digitalWrite(10,LOW);
	}

	//-------------------------------- There is nothing to display, turn of status LED.
	else 
	{
		digitalWrite(9,LOW);
		digitalWrite(11,LOW);
		digitalWrite(10,LOW);
	}
	
	//-------------------------------- print battery graphic based on voltage level
	_lcd.setCursor(0,0);
	_lcd.write(char(floor(voltage*2)-20));
	
	//-------------------------------- print current law of operation
	_lcd.setCursor(3,0);
	if(law == 0x00)
	{
		_lcd.print("Direct");
	} else if(law == 0x02)
	{
		_lcd.print("Prec.");
	}
	
	//-------------------------------- print horizontal movement of crane
	_lcd.setCursor(13,1);
	if(stateX < -1) _lcd.print("<"); else if(stateX == -1) _lcd.print("("); else if(stateX == 0) _lcd.print("|"); else if(stateX == 1) _lcd.print(")"); else _lcd.print(">");
	
	//-------------------------------- print vertical movement of crane
	_lcd.setCursor(15,1);
	if(stateY < -1) _lcd.print("_"); else if(stateY == -1) _lcd.print(","); else if(stateY == 0) _lcd.print("-"); else if(stateY == 1) _lcd.print("'"); else _lcd.print("^");
	
	//-------------------------------- print the status string
	_lcd.setCursor(0,1);
	_lcd.print(statusString);
	
	
	
}

/// The verify function for arduino 3
/// Displays verification outcome, and test the LEDs
/// 
int Crane::verify3()
{
	
	//-------------------------------- Display "Starting..."
	_lcd.clear();
	_lcd.home();
	_lcd.write("Starting...");
	digitalWrite(9,HIGH);
	digitalWrite(10,HIGH);
	digitalWrite(11,HIGH);
	delay(400);
	digitalWrite(9,LOW);
	digitalWrite(10,LOW);
	digitalWrite(11,LOW);
	
	//-------------------------------- While Arduinos 1 and 2 verify, display a line graphic.
	for(int x = 0; x < 16; x++)
	{ _lcd.setCursor(x,1); _lcd.write("-"); delay(600); if(x == 8) pushBuffer(1); }

	//-------------------------------- Wait a bit more
	delay(4500);
	_lcd.clear();
	_lcd.home();
	
	//-------------------------------- If the board has verified, show "I2C: PASS"
	if(boardVerified) 
	_lcd.write("I2C: PASS");
	
	//-------------------------------- if the board has not been verified, show "I2C: FAIL"
	else
	{ _lcd.write("I2C: FAIL"); statusString = "I2C Fail"; }

	//-------------------------------- display HC06 Status
	_lcd.setCursor(0,1);
	if(blueState == 0)
		_lcd.print("HC-06: --"); //Bluetooth neither confirmed working or fail
	if(blueState == 1)
		_lcd.print("HC-06: OK"); //Bluetooth confirmed working
	if(blueState == 2)
		_lcd.print("HC-06: ERR"); //Bluetooth legible, but not working properly
	if(blueState == 3)
		_lcd.print("HC-06: INOP"); //Bluetooth confirmed fail
	
	//-------------------------------- Cycle the RGB LED
	delay(2000);
	digitalWrite(9,HIGH);
	digitalWrite(10,LOW);
	digitalWrite(11,LOW);
	delay(1000);
	digitalWrite(9,LOW);
	digitalWrite(10,HIGH);
	digitalWrite(11,LOW);
	delay(1000);
	digitalWrite(9,LOW);
	digitalWrite(10,LOW);
	digitalWrite(11,HIGH);
	delay(1000);
	digitalWrite(9,LOW);
	digitalWrite(10,LOW);
	digitalWrite(11,LOW);
	
	
	
}

/// Creates the custom characters for the LCD screen
/// 
/// Note: call this function only once!!
void Crane::createCustomChars()
{
	//-------------------------------- declares the temporary variable
	byte customChar[] = {0, 0, 0, 0, 0, 0, 0, 0};
	
	//-------------------------------- defines the graphic
	customChar[0] = B01110;
	customChar[1] = B10001;
	customChar[2] = B10001;
	customChar[3] = B10001;
	customChar[4] = B10001;
	customChar[5] = B10001;
	customChar[6] = B10001;
	customChar[7] = B11111;
	
	//-------------------------------- Output graphic to LCD
	_lcd.createChar(0,customChar);
	delay(100);
	
	//-------------------------------- defines the graphic
	customChar[0] = B01110;
	customChar[1] = B10001;
	customChar[2] = B10001;
	customChar[3] = B10001;
	customChar[4] = B10001;
	customChar[5] = B11111;
	customChar[6] = B11111;
	customChar[7] = B11111;
	
	//-------------------------------- Output graphic to LCD
	_lcd.createChar(1,customChar);
	delay(100);
	
	//-------------------------------- defines the graphic
	customChar[0] = B01110;
	customChar[1] = B10001;
	customChar[2] = B10001;
	customChar[3] = B11111;
	customChar[4] = B11111;
	customChar[5] = B11111;
	customChar[6] = B11111;
	customChar[7] = B11111;
	
	//-------------------------------- Output graphic to LCD
	_lcd.createChar(2,customChar);
	delay(100);
	
	//-------------------------------- defines the graphic
	customChar[0] = B01110;
	customChar[1] = B11111;
	customChar[2] = B11111;
	customChar[3] = B11111;
	customChar[4] = B11111;
	customChar[5] = B11111;
	customChar[6] = B11111;
	customChar[7] = B11111;
	
	//-------------------------------- Output graphic to LCD
	_lcd.createChar(3,customChar);
	delay(100);
	
	//-------------------------------- defines the graphic
	customChar[0] = B00000;
	customChar[1] = B00000;
	customChar[2] = B00000;
	customChar[3] = B00000;
	customChar[4] = B00000;
	customChar[5] = B00000;
	customChar[6] = B00000;
	customChar[7] = B01000;
	
	//-------------------------------- Output graphic to LCD
	_lcd.createChar(4,customChar);
	delay(100);
	
	//-------------------------------- defines the graphic
	customChar[0] = B00000;
	customChar[1] = B00000;
	customChar[2] = B00000;
	customChar[3] = B00000;
	customChar[4] = B00000;
	customChar[5] = B00000;
	customChar[6] = B00010;
	customChar[7] = B01010;
	
	//-------------------------------- Output graphic to LCD
	_lcd.createChar(5,customChar);
	delay(100);
	
	//-------------------------------- defines the graphic
	customChar[0] = B00000;
	customChar[1] = B00000;
	customChar[2] = B00000;
	customChar[3] = B00000;
	customChar[4] = B00000;
	customChar[5] = B10000;
	customChar[6] = B10000;
	customChar[7] = B10000;
	
	//-------------------------------- Output graphic to LCD
	_lcd.createChar(6,customChar);
	delay(100);
	
	//-------------------------------- defines the graphic
	customChar[0] = B00000;
	customChar[1] = B00000;
	customChar[2] = B00000;
	customChar[3] = B00000;
	customChar[4] = B00100;
	customChar[5] = B10100;
	customChar[6] = B10100;
	customChar[7] = B10100;
	
	//-------------------------------- Output graphic to LCD
	_lcd.createChar(7,customChar);
	delay(100);
  
}
