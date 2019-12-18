#include <Servo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <Encoder.h>

/*
 * DynaTable Arduino Program
 * Version 7
 * C. Kos, B. Cheizoo, J. de Wolde, T. Hartsuijker
 */

// Declaration of structures

struct motorDC {
  String mname;
  float cRE, limit, mmptick;
  int dirRef, currentState, rREPrev;
};

struct motorSV {
  float limit;
  int currentState;
};

// Initialization of constants

const long Tpress = 1000;

const int L = 5; // Number of levels
const int A = 20; // Maximum amplitude (mm)

const float Tservo = 500; // Servo time in ms (min. 450 ms)

const int DCXM1 = 2; // Motor pins
const int DCXM2 = 3;
const int DCYM1 = 4;
const int DCYM2 = 5;
/*
const int SV3 = ?;
const int SV4 = ?;
*/
const int ENABLE = 8; // Enable pin

const int ROTENCXA = 11; // Rotary encoder pins
const int ROTENCXB = 12;
const int ROTENCYA = 22;
const int ROTENCYB = 23;

// Initialization of variables used in the main functions and the FSM

bool mode = 0; // Continuous mode if 'true', pulse mode if 'false'
bool enable = 0; // In pulse mode, start cycle if 'true'

bool changed = 0;

unsigned long tmode = 0;
bool timer = false;

// Initialization of motor structures

motorDC m1 = {"m1", 0, 0, (57.1-10.1)/1779, 0, 0, 0};
motorDC m2 = {"m2", 0, 0, (52.9-15)/3078, 0, 0, 0};

//Servo sv3;
//motorSV m3 = {90, 0};

// Initialization rotary encoder
Encoder rotEncX(ROTENCXA, ROTENCXB);
Encoder rotEncY(ROTENCYA, ROTENCYB);

void setup() {
  Serial.begin(115200);
  Serial.println("DynaTable Arduino Program");
  Serial.println("For Arduino Mega or Arduino Mega 2560");
  Serial.println("Version 7");
  Serial.println();
  Serial.println("C. Kos, B. Cheizoo, J. de Wolde, T. Hartsuijker");
  Serial.println();
  
  pinMode(ENABLE, INPUT);
  
	pinMode(DCXM2, OUTPUT);
	pinMode(DCXM1, OUTPUT);
  
  pinMode(DCYM2, OUTPUT);
  pinMode(DCYM1, OUTPUT);
/*
  sv3.attach(SV3);
  sv3.write(90);
*/
  
}

void loop() {

  enable = digitalRead(ENABLE);

  if (enable == true && changed == false) {
    if (timer == false) {
      tmode = millis();
      timer = true;
    }
    
    if (millis() >= tmode + Tpress) {
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
    if (enable == false && changed == true) {
      changed = false;
    }
    timer = false;
  }

  m1 = stateMachineDC(m1);

  int rREX = rotEncX.read();
//  if (rREX != m1.rREPrev) {
    m1.rREPrev = rREX;
    m1.cRE += m1.dirRef;
//  }

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

  m2 = stateMachineDC(m2);

  int rREY = rotEncY.read();
//  if (rREY != m2.rREPrev) {
    m2.rREPrev = rREY;
    m2.cRE += m2.dirRef;
//  }

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
/*
  m3 = stateMachineSV(m3);

  sv3.write(m3.limit);
*/

}
