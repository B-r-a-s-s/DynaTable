#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Initialization of constants

const int a = 2;	// Maximum amplitude

// Initialization of variables used in the main functions and the FSM

bool mode = false; // Continuous mode if 'true', pulse mode if 'false'
bool enable = true; // In pulse mode, start cycle if 'true'

double z1 = 0;		// Current displacement in the z-direction
double z2 = 0;

int currentState1 = 0;		// Current state
int currentState2 = 0;

double limit1 = 0;	// Current destination limit
double limit2 = 0;

int dirRef1 = 0;		// Current motor direction, clockwise if equal to 1, counterclockwise if equal to -1 and off if equal to 0
int dirRef2 = 0;

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

void stateMachine1() {
	switch (currentState1) {
		case 0:
		
		if (mode == false || enable == true) {
			while (limit1 == 0) {
				
				limit1 = (rand() % (2*a+1)) - a;
				
			}
			
			if (limit1 > 0) {
				dirRef1 = 1;
				currentState1 = 1;
			}else if (limit1 < 0) {
				dirRef1 = -1;
				currentState1 = 3;
			}else{
				dirRef1 = 0;
				currentState1 = 0;
			}
			
		}else{
			currentState1 = 0;
			limit1 = 0;
			dirRef1 = 0;
		}
		
		break;

		case 1:
		
		if (z1 >= limit1) {
			currentState1 = 2;
			limit1 = 0;
			dirRef1 = -1;
		}else{
			currentState1 = 1;
			limit1 = limit1;
			dirRef1 = 1;
		}
		
		break;

		case 2:
		
		if (z1 <= limit1) {
			currentState1 = 0;
			limit1 = 0;
			dirRef1 = 0;
		}else{
			currentState1 = 2;
			limit1 = 0;
			dirRef1 = -1;
		}
		
		break;

		case 3:
		
		if (z1 <= limit1) {
			currentState1 = 4;
			limit1 = 0;
			dirRef1 = 1;
		}else{
			currentState1 = 3;
			limit1 = limit1;
			dirRef1 = -1;
		}
		
		break;

		case 4:
		
		if (z1 >= limit1) {
			currentState1 = 0;
			limit1 = 0;
			dirRef1 = 0;
		}else{
			currentState1 = 4;
			limit1 = 0;
			dirRef1 = 1;
		}
		
		break;
		
	}
  
}

void stateMachine2() {
  switch (currentState2) {
    case 0:
    
    if (mode == false || enable == true) {
      while (limit2 == 0) {
        
        limit2 = random(-a, a);
        
      }
      
      if (limit2 > 0) {
        dirRef2 = 1;
        currentState2 = 1;
      }else if (limit2 < 0) {
        dirRef2 = -1;
        currentState2 = 3;
      }else{
        dirRef2 = 0;
        currentState2 = 0;
      }
      
    }else{
      currentState2 = 0;
      limit2 = 0;
      dirRef2 = 0;
    }
    
    break;

    case 1:
    
    if (z2 >= limit2) {
      currentState2 = 2;
      limit2 = 0;
      dirRef2 = -1;
    }else{
      currentState2 = 1;
      limit2 = limit2;
      dirRef2 = 1;
    }
    
    break;

    case 2:
    
    if (z2 <= limit2) {
      currentState2 = 0;
      limit2 = 0;
      dirRef2 = 0;
    }else{
      currentState2 = 2;
      limit2 = 0;
      dirRef2 = -1;
    }
    
    break;

    case 3:
    
    if (z2 <= limit2) {
      currentState2 = 4;
      limit2 = 0;
      dirRef2 = 1;
    }else{
      currentState2 = 3;
      limit2 = limit2;
      dirRef2 = -1;
    }
    
    break;

    case 4:
    
    if (z2 >= limit2) {
      currentState2 = 0;
      limit2 = 0;
      dirRef2 = 0;
    }else{
      currentState2 = 4;
      limit2 = 0;
      dirRef2 = 1;
    }
    
    break;
    
  }
  
}

void setup() {
	pinMode(13, OUTPUT);
	pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);

	Serial.begin(9600);
  
}

void loop() {
  Serial.print("currentState1: ");
  Serial.print(currentState1);
  Serial.print(" | z1: ");
  Serial.print(z1);
  Serial.print(" | limit1: ");
  Serial.print(limit1);
  Serial.print(" | dirRef1: ");
  Serial.print(dirRef1);
  Serial.print("\n");

  stateMachine1();
  
  z1 = z1 + dirRef1;
  
	Serial.print("currentState2: ");
	Serial.print(currentState2);
	Serial.print(" | z2: ");
	Serial.print(z2);
	Serial.print(" | limit2: ");
	Serial.print(limit2);
	Serial.print(" | dirRef2: ");
	Serial.print(dirRef2);
	Serial.print("\n");

	stateMachine2();
  
	z2 = z2 + dirRef2;
  
//	switch (dirRef1) {
//		case -1:
//		
//		digitalWrite(13, LOW);
//		digitalWrite(12, HIGH);
//		
//		break;
//
//		case 0:
//		
//		digitalWrite(13, LOW);
//		digitalWrite(12, LOW);
//		
//		break;
//
//		case 1:
//		
//		digitalWrite(13, HIGH);
//		digitalWrite(12, LOW);
//		
//		break;
//		
//	}
//
//  switch (dirRef2) {
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

  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  
	delay(1000);
  
}
