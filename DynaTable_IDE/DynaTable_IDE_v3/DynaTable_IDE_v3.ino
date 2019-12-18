#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Declaration of structures

struct motorDC {
  float z, limit, v;
  int dirRef, currentState;
};

// Initialization of constants

const int L = 5; // Number of levels

const int D = 500; // Time step size in ms

const float A = 0.02; // Maximum amplitude

// Initialization of variables used in the main functions and the FSM

bool mode = false; // Continuous mode if 'true', pulse mode if 'false'
bool enable = true; // In pulse mode, start cycle if 'true'

// Initialization of motor structures

motorDC m1 = {0, 0, 0.01, 0, 0};
//motorDC m2 = {0, 0, 0.01, 0, 0};

// Rotational direction calculated from the outputs of the rotary encoder

//bool rotEncAPrev = false;
//bool rotDirPrev = false;
//
//bool rotDir (bool rotEncA, bool rotEncB) {
//	bool rotDir = 0;
//	
//	if (rotEncA != rotEncAPrev) {
//		rotDir = rotEncA^rotEncB && rotEncA^rotEncAPrev;
//	}else{
//		rotDir = rotDirPrev;
//	}
//	rotEncAPrev=rotEncA;
//	rotDirPrev=rotDir;
//	
//	return rotDir;
//  
//}

// The FSM used to determine the output of the motor

motorDC stateMachine(motorDC mTemp) {
  
	switch (mTemp.currentState) {
		case 0:
		
		if (mode == false || enable == true) {
			while (mTemp.limit == 0) {
				
				mTemp.limit = 2*A*random(-(L-1)/2,(L-1)/2+1)/(L-1);
				
			}
			
			if (mTemp.limit > 0) {
				mTemp.dirRef = 1;
				mTemp.currentState = 1;
			}else if (mTemp.limit < 0) {
				mTemp.dirRef = -1;
				mTemp.currentState = 3;
			}else{
				mTemp.dirRef = 0;
				mTemp.currentState = 0;
			}
			
		}else{
			mTemp.currentState = 0;
			mTemp.limit = 0;
			mTemp.dirRef = 0;
		}
		
		break;

		case 1:
		
		if (mTemp.z >= mTemp.limit) {
			mTemp.currentState = 2;
			mTemp.limit = 0;
			mTemp.dirRef = -1;
		}else{
			mTemp.currentState = 1;
			mTemp.limit = m1.limit;
			mTemp.dirRef = 1;
		}
		
		break;

		case 2:
		
		if (mTemp.z <= mTemp.limit) {
			mTemp.currentState = 0;
			mTemp.limit = 0;
			mTemp.dirRef = 0;
		}else{
			mTemp.currentState = 2;
			mTemp.limit = 0;
			mTemp.dirRef = -1;
		}
		
		break;

		case 3:
		
		if (mTemp.z <= mTemp.limit) {
			mTemp.currentState = 4;
			mTemp.limit = 0;
			mTemp.dirRef = 1;
		}else{
			mTemp.currentState = 3;
			mTemp.limit = m1.limit;
			mTemp.dirRef = -1;
		}
		
		break;

		case 4:
		
		if (mTemp.z >= mTemp.limit) {
			mTemp.currentState = 0;
			mTemp.limit = 0;
			mTemp.dirRef = 0;
		}else{
			mTemp.currentState = 4;
			mTemp.limit = 0;
			mTemp.dirRef = 1;
		}
		
		break;
		
	}

  return mTemp;
  
}

void setup() {
	pinMode(13, OUTPUT);
	pinMode(12, OUTPUT);
  
//  pinMode(11, OUTPUT);
//  pinMode(10, OUTPUT);

	Serial.begin(9600);
  
}

void loop() {
  Serial.print("m1.currentState: ");
  Serial.print(m1.currentState);
  Serial.print(" | m1.z: ");
  Serial.print(m1.z);
  Serial.print(" | m1.limit: ");
  Serial.print(m1.limit);
  Serial.print(" | m1.dirRef: ");
  Serial.print(m1.dirRef);

  m1 = stateMachine(m1);
  
  m1.z = m1.z + m1.dirRef*m1.v;

  Serial.print("\n");
  
//  Serial.print("m2.currentState: ");
//  Serial.print(m2.currentState);
//  Serial.print(" | m2.z: ");
//  Serial.print(m2.z);
//  Serial.print(" | m2.limit: ");
//  Serial.print(m2.limit);
//  Serial.print(" | m2.dirRef: ");
//  Serial.print(m2.dirRef);
//
//	m2 = stateMachine(m2);
//  
//	m2.z = m2.z + m2.dirRef;
//  
//  Serial.print("\n");
  
	switch (m1.dirRef) {
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

//  switch (m2.dirRef) {
//    case -1:
//    
//    digitalWrite(11, LOW);
//    digitalWrite(10, HIGH);
//    
//    break;
//
//    case 0:
//    
//    digitalWrite(11, LOW);
//    digitalWrite(10, LOW);
//    
//    break;
//
//    case 1:
//    
//    digitalWrite(11, HIGH);
//    digitalWrite(10, LOW);
//    
//    break;
//    
//  }

//  digitalWrite(13, LOW);
//  digitalWrite(12, LOW);
//  
//  digitalWrite(11, LOW);
//  digitalWrite(10, LOW);
  
	delay(D);
  
}
