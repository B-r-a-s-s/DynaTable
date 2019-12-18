#include <Servo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Declaration of structures

struct motorDC {
  float z, limit, v;
  int dirRef, currentState;
};

struct motorSV {
  float pos, limit, v;
  int dirRef, currentState;
};

// Initialization of constants

const int L = 5; // Number of levels

const float D = 100; // Time step size in ms

const float A = 0.02; // Maximum amplitude

//const int DC1M1 = 12; // Motor pins
//const int DC1M2 = 13;
//const int DC2M1 = 10;
//const int DC2M2 = 11;

const int SV3 = 13 ;
//const int SV4 = ?;

const int ENABLE = 2; // Enable pin

// Initialization of variables used in the main functions and the FSM

int mode = 0; // Continuous mode if 'true', pulse mode if 'false'
int enable = 0; // In pulse mode, start cycle if 'true'

// Initialization of motor structures

//motorDC m1 = {0, 0, 0.01, 0, 0};
//motorDC m2 = {0, 0, 0.01, 0, 0};

//Servo sv3;

motorSV m3 = {90, 90, 1, 0, 0};

void setup() {
  pinMode(ENABLE, INPUT);
  
//	pinMode(DC1M2, OUTPUT);
//	pinMode(DC1M1, OUTPUT);
  
//  pinMode(DC2M2, OUTPUT);
//  pinMode(DC2M1, OUTPUT);

  sv3.attach(SV3);

	Serial.begin(9600);

  digitalWrite(13, LOW);
//  digitalWrite(12, LOW);

  digitalWrite(2, LOW);
  
}

int i = 0;
int changed = 0;

void loop() {

  enable = digitalRead(ENABLE);

  if (enable == 1 && changed == 0) {
    i++;
    if (i > 10) {
      if (mode == 1) {
        mode = 0;
      }else if (mode == 0) {
        mode = 1;
      }else{
        mode = mode;
      }
      changed = 1;
    }
  }else{
    i = 0;
    if (enable == 0 && changed == 1) {
      changed = 0;
    }
  }

//  if (enable == 0) {
//    changed = 0;
//  }
//  
//  Serial.print("m1.currentState: ");
//  Serial.print(m1.currentState);
//  Serial.print(" | m1.z: ");
//  Serial.print(m1.z);
//  Serial.print(" | m1.limit: ");
//  Serial.print(m1.limit);
//  Serial.print(" | m1.dirRef: ");
//  Serial.print(m1.dirRef);
//
//  m1 = stateMachineDC(m1);
//  
//  m1.z = m1.z + (m1.dirRef*m1.v*(D/1000));
//
//  Serial.print("\n");
//  
//  Serial.print("m2.currentState: ");
//  Serial.print(m2.currentState);
//  Serial.print(" | m2.z: ");
//  Serial.print(m2.z);
//  Serial.print(" | m2.limit: ");
//  Serial.print(m2.limit);
//  Serial.print(" | m2.dirRef: ");
//  Serial.print(m2.dirRef);
//
//  mode = digitalRead(MODE);
//  enable = digitalRead(ENABLE);
//
//	m2 = stateMachine(m2);
//  
//	m2.z = m2.z + m2.dirRef;
//  
//  Serial.print("\n");
//  
//	switch (m1.dirRef) {
//		case -1:
//		
//		digitalWrite(DC1M2, LOW);
//		digitalWrite(DC1M1, HIGH);
//		
//		break;
//
//		case 0:
//		
//		digitalWrite(DC1M2, LOW);
//		digitalWrite(DC1M1, LOW);
//		
//		break;
//
//		case 1:
//		
//		digitalWrite(DC1M2, HIGH);
//		digitalWrite(DC1M1, LOW);
//		
//		break;
//		
//	}

//  switch (m2.dirRef) {
//    case -1:
//    
//    digitalWrite(DC2M2, LOW);
//    digitalWrite(DC2M1, HIGH);
//    
//    break;
//
//    case 0:
//    
//    digitalWrite(DC2M2, LOW);
//    digitalWrite(DC2M1, LOW);
//    
//    break;
//
//    case 1:
//    
//    digitalWrite(DC2M2, HIGH);
//    digitalWrite(DC2M1, LOW);
//    
//    break;
//    
//  }

//  digitalWrite(DC1M2, LOW);
//  digitalWrite(DC1M1, LOW);
//  
//  digitalWrite(DC2M2, LOW);
//  digitalWrite(DC2M1, LOW);

  Serial.print("m3.currentState: ");
  Serial.print(m3.currentState);
  Serial.print(" | m3.pos: ");
  Serial.print(m3.pos);
  Serial.print(" | m3.limit: ");
  Serial.print(m3.limit);
  Serial.print(" | m3.dirRef: ");
  Serial.print(m3.dirRef);

  mode = digitalRead(MODE);
  enable = digitalRead(ENABLE);

  m3 = stateMachineSV(m3);
  
  m3.pos = m3.pos + m3.dirRef*m3.v;

  sv3.write(m3.limit);
  
	delay(D);
  
}
