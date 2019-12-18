#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Initialization of constants

const int N = 3;	// Number of cycles
const int A = 2;	// Maximum amplitude

// Initialization of variables used in the main functions and the FSM

double z = 0;		// Current displacement in the z-direction
bool reset = false;	// Continuous mode if 'true', pulse mode if 'false'
bool enable = true; // In pulse mode, start cycle if 'true'
int state = 0;		// Current state
double limit = 0;	// Current destination limit
int dirRef = 0;		// Current motor direction, clockwise if equal to 1, counterclockwise if equal to -1 and off if equal to 0
int i = 1;			// Current cycle number

// Rotational direction calculated from the outputs of the rotary encoder

/* bool rotEncAPrev = false;
bool rotDirPrev = false;

bool rotDir (bool rotEncA, bool rotEncB) {
	bool rotDir = 0;
	
	if (rotEncA != rotEncAPrev) {
		rotDir = rotEncA^rotEncB && rotEncA^rotEncAPrev;
	}else{
		rotDir = rotDirPrev;
	}
	rotEncAPrev=rotEncA;
	rotDirPrev=rotDir;
	
	return rotDir;
  
} */

// The FSM used to determine the direction of the motor

void stateMachine() {
	switch (state) {
		case 0:
		
		if (reset == false || enable == true) {
			while (limit == 0) {
				
				limit = (rand() % (2*A+1)) - A;
				
			}
			
			if (limit > 0) {
				dirRef = 1;
				state = 1;
			}else if (limit < 0) {
				dirRef = -1;
				state = 3;
			}else{
				dirRef = 0;
				state = 0;
			}
			
		}else{
			state = 0;
			limit = 0;
			dirRef = 0;
		}
		
		break;

		case 1:
		
		if (z >= limit) {
			state = 2;
			limit = 0;
			dirRef = -1;
		}else{
			state = 1;
			limit = limit;
			dirRef = 1;
		}
		
		break;

		case 2:
		
		if (z <= limit) {
			i++;
			state = 0;
			limit = 0;
			dirRef = 0;
		}else{
			state = 2;
			limit = 0;
			dirRef = -1;
		}
		
		break;

		case 3:
		
		if (z <= limit) {
			state = 4;
			limit = 0;
			dirRef = 1;
		}else{
			state = 3;
			limit = limit;
			dirRef = -1;
		}
		
		break;

		case 4:
		
		if (z >= limit) {
			i++;
			state = 0;
			limit = 0;
			dirRef = 0;
		}else{
			state = 4;
			limit = 0;
			dirRef = 1;
		}
		
		break;
		
	}

	return;
  
}

void setup() {
	pinMode(13, OUTPUT);
	pinMode(12, OUTPUT);

	Serial.begin(9600);
  
}

void loop() {
	while (i <= N) {
		Serial.print("i: ");
		Serial.print(i);
		Serial.print(" | state: ");
		Serial.print(state);
		Serial.print(" | z: ");
		Serial.print(z);
		Serial.print(" | limit: ");
		Serial.print(limit);
		Serial.print(" | dirRef: ");
		Serial.print(dirRef);
		Serial.print("\n");
  
		stateMachine();
    
		z = z + dirRef;
    
		switch (dirRef) {
			case -1:
			
			digitalWrite(13, LOW);
			digitalWrite(12, HIGH);
			
			break;
  
			case 0:
			
			digitalWrite(13, LOW);
			digitalWrite(12, LOW);
			
			break;
  
			case 1:
			
			digitalWrite(13, HIGH);
			digitalWrite(12, LOW);
			
			break;
			
		}
    
		delay(1000);
		
	}

	digitalWrite(13, LOW);
	digitalWrite(12, LOW);
  
}
