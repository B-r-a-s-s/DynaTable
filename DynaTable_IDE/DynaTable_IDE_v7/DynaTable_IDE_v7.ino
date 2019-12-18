#include <Servo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <Encoder.h>

// Declaration of structures

struct motorDC {
  float cRE, limit;
  int dirRef, currentState, rREPrev;
};

struct motorSV {
  float limit;
  int currentState;
};

// Initialization of constants

const int L = 5; // Number of levels

const float Tservo = 500; // Servo time in ms (min. 450 ms)

const float mmptickx = (57.1-10.1)/1779; // Rate

const int DCXM1 = 2; // Motor pins
const int DCXM2 = 3;
const int DCYM1 = 4;
const int DCYM2 = 5;
/*
const int SV3 = ?;
const int SV4 = ?;
*/
const int ENABLE = 13; // Enable pin

const int ROTENCXA = 11; // Rotary encoder pins
const int ROTENCXB = 12;

// Initialization of variables used in the main functions and the FSM

bool mode = 0; // Continuous mode if 'true', pulse mode if 'false'
bool enable = 0; // In pulse mode, start cycle if 'true'

// Initialization of motor structures

motorDC m1 = {0, 0, 0, 0};
//motorDC m2 = {0, 0, 0, 0};

//Servo sv3;
//motorSV m3 = {90, 0};

// Initialization rotary encoder
Encoder rotEncX(ROTENCXA, ROTENCXB);

void setup() {
  pinMode(ENABLE, INPUT);
  
	pinMode(DCXM2, OUTPUT);
	pinMode(DCXM1, OUTPUT);
/*  
  pinMode(DCYM2, OUTPUT);
  pinMode(DCYM1, OUTPUT);

  sv3.attach(SV3);
  sv3.write(90);
*/
	Serial.begin(115200);
  Serial.println("Test");
  
}

unsigned long i = 0;
bool changed = 0;

unsigned long tmode = 0;
bool timer = false;
const long Dpress = 1000;

void loop() {

  enable = digitalRead(ENABLE);

  if (enable == true && changed == false) {
    if (timer == false) {
      tmode = millis();
      timer = true;
    }
    
    if (millis >= tmode + Dpress) {
      if (mode == true) {
        mode = false;
      }else if (mode == false) {
        mode = true;
      }else{
        mode = mode;
      }
      changed = true;
      timer = false;
    }
  }else{
    i = 0;
    if (enable == false && changed == true) {
      changed = false;
    }
    timer = false;
  }

  m1 = stateMachineDC(m1);
/*
	m2 = stateMachineDC(m2);
*/
	switch (m1.dirRef) {
		case -1:
		
		digitalWrite(DCXM2, LOW);
		digitalWrite(DCXM1, HIGH);
    		
		break;

		case 0:
		
		digitalWrite(DCXM2, LOW);
		digitalWrite(DCXM1, LOW);
		
		break;

		case 1:
		
		digitalWrite(DCXM2, HIGH);
		digitalWrite(DCXM1, LOW);
		
		break;
		
	}
/*
  switch (m2.dirRef) {
    case -1:
    
    digitalWrite(DCYM2, LOW);
    digitalWrite(DCYM1, HIGH);
    
    break;

    case 0:
    
    digitalWrite(DCYM2, LOW);
    digitalWrite(DCYM1, LOW);
    
    break;

    case 1:
    
    digitalWrite(DCYM2, HIGH);
    digitalWrite(DCYM1, LOW);
    
    break;
    
  }

  m3 = stateMachineSV(m3);

  sv3.write(m3.limit);
*/

  int rREX = rotEncX.read();
  if (rREX != m1.rREPrev) {
    m1.rREPrev = rREX;
    m1.cRE += m1.dirRef;
  }
/*
  Serial.print("m1.currentState: ");
  Serial.print(m1.currentState);
  Serial.print(" | m1.z: ");
  Serial.print(m1.z, 5);
  Serial.print(" | m1.limit: ");
  Serial.print(m1.limit, 5);
  Serial.print(" | m1.dirRef: ");
  Serial.print(m1.dirRef);
  Serial.print(" | cREX: ");
  Serial.println(cREX);

  m2.z += (m2.dirRef*m2.v*(D/1000));

  Serial.print("m2.currentState: ");
  Serial.print(m2.currentState);
  Serial.print(" | m2.z: ");
  Serial.print(m2.z, 4);
  Serial.print(" | m2.limit: ");
  Serial.print(m2.limit, 4);
  Serial.print(" | m2.dirRef: ");
  Serial.println(m2.dirRef);

  Serial.print("m3.currentState: ");
  Serial.print(m3.currentState);
  Serial.print(" | m3.limit: ");
  Serial.println(m3.limit, 3);
*/
  ;

}
