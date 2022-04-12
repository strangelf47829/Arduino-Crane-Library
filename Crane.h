#ifndef Crane_h
#define Crane_h


#include <inttypes.h>
#include "C:\Program Files (x86)\Arduino\libraries\LiquidCrystal\src\LiquidCrystal.h"
#include "Arduino.h"
#include "Wire.h"
#include "LiquidCrystal.h"
#include "Stepper.h"
#include "Servo.h"
#include "SoftwareSerial.h"

class Crane
{
	private:
	
	
		//Shared Functions
		void subscribe(uint8_t index); 												//Subscribed indexes will be pushed next time.
		int LCM(float n1, float n2, float n3); 										//Returns the LCM of 3 variables. Not efficient
		uint8_t maX(uint8_t a, uint8_t b); 											//Returns a if a > b, or b if a<=b
		
		//Shared Variables									
		uint8_t _arduinoID; 														//Arduino 1, 2 or 3
		bool devMode = true; 														//Outputs several things to the LCD screen if in devMode.
		bool subscribed[16];														//The list to track which elements are subscribed
	    String _instrBuffer[16];													//The buffer
		int blueState = 0;															//-1: check (arduino 1), 0: not set, 1: blueOK, 2: blueERR, 3: blueINOP
		
		//Private functions arduino 1							
		int init1(); 																//Initializes the arduino
		void update1();																//Runs on the loop of arduino 1
		int verify1();																//Verifies the board
		
		//Private variables arduino 1									
		bool arduino2Verify; 														//True if arduino 1 has received a ping reply from arduino 2
		bool arduino3Verify; 														//True if arduino 1 has received a ping reply from arduino 3
		String in; 																	//The string that arduino 1 uses and parses from the HC-06
		
		
		
		//Private functions arduino 2									
		int verify2();																//Initializes the arduino
		void update2();																//Runs on the loop of arduino 2
		int init2();																//Verifies the stepper motors, and return the ping from arduino 1.
		
		//Private variables arduino 2									
		uint8_t delayStep1 = 0;														//The amount of milliseconds in between each step of stepper motor 1
		uint8_t delayStep2 = 0;														//The amount of milliseconds in between each step of stepper motor 2
		uint8_t delayStep3 = 0;														//The amount of milliseconds in between each step of stepper motor 3
		
		
		
		//Private functions arduino 3									
		int init3();																//Initializes the arduino
		void update3();																//Runs on the loop of arduino 3
		int verify3();																//Verifies LCD, and return the ping from arduino 1.
		void createCustomChars();													//Creates the custom characters for the LCD
		void calculatePidValues();													//Calculates the PID values for arduino 1 to use (EXPERIMENTAL W.I.P.)
		
		//Private variables arduino 3							
		int frameConnect = 0;														//A variable used only for the connection graphic on the LCD display.
		
		
		
	public:							
		
		//Public functions arduino 1							
		String returnHC06Msg();														//Returns the last message sent from the HC-06
		
		//Public variables arduino 1							
		SoftwareSerial hcSerial {3, 2}; 											//The SoftwareSerial object. Used for communications with the HC-06
		Servo grip;																	//The servo object of the arduino. used to actuate the gripper.
		float voltage = 11;
		
		//Public functions arduino 2							
		void step(uint8_t stepper, bool direction); 								//Continuously spins stepper in direction
		void step(uint8_t stepper); 												//Steps the motor once.
		void stepTo(uint8_t stepper, double positionFrom, double positionTo);
		double setSpeedOf(uint8_t stepper, float rps); 								//sets the closest mode, and returns the deltaT;
		void stepSync();
		
		//Public variables arduino 2
		int runTime = 1;															//The runtime of the current stepper "event" (used in the stepSync function)
		
		
		
		//Public functions arduino 3
		
		//Public variables arduino 3
		String statusString = "ERR";												//The current status of the crane. "ERR" = uninitialized
		LiquidCrystal _lcd {8, 12, 4, 5, 6, 7};										//The LCD object
		
		
		
		//Shared Variables							
		uint8_t  bufferIndex;														//The current index of the buffer
		uint8_t  bufferLength;														//The pseudo length of the buffer
		bool boardVerified;															//True if verify(), verify1(), verify2(), verify3() were all completed successfully
		bool blueConnected = false;													//True if the bluetooth has been connected
		int stateX;																	//The state of horizontal movement of the crane
		int stateY;																	//The state of vertical movement of the crane
		int law = 1;																//0: Direct law, 1: normal law, 2: precision law
		
		
		//Shared Functions							
		Crane(uint8_t arduinoID);													//The constructor of this class
		int verify();																//Verifies the components and the board
		int init();																	//Initializes the components and the board
		void instruct();															//Unused: Instructs an arduino to do something
		void monitor();																//Unused: Monitors the situation
		void update();																//Runs on the loop. For miscellaneous tasks
		
		void sendData(uint8_t arduino, byte data);									//Sends a byte of data to an arduino over I2C.
		void sendData(uint8_t arduino, String data);								//Sends a string of data to an arduino over I2C.
		
		String onReceive(int bytes);												//Used to automatically parse things, and returns the parsed string.
		
		void flushBuffer();															//Clears the buffer of all data
		void addToBuffer(String instruction);										//Adds an element to the buffer
		void pushBuffer(uint8_t arId, uint8_t n);									//Pushes n amount of subscribed strings to an arduino over I2C and removes those items from the buffer
		void pushBuffer(uint8_t arId);												//Pushes subscribed elements to an arduino over I2C.
		void removeNFromBuffer(uint8_t n);											//Removes n amount of elements from the buffer
		int available();															//Returns the amount of unread elements in the buffer.
		String readBuffer();														//Reads the first unread element, and removes it from the buffer.
};



#endif