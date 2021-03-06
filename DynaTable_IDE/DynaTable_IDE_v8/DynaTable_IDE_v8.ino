#include <Servo.h>
#include <math.h>
//#include <stdio.h>
//#include <stdlib.h>
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
  int currentState, rREPrev, add, p1, p2, pulse;
  unsigned long halt;
};

struct motorSV {
  float limit;
  int currentState;
};

// Initialization of constants

const long Tpress = 1000;

const int L = 5; // Number of levels
const int A = 5; // Maximum amplitude (mm)

const float Tservo = 500; // Servo time in ms (min. 450 ms)
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

bool changed = false;

unsigned long tmode = 0;
bool timer = false;
int pulseFlag = 0;

// Initialization of motor structures

motorDC m1 = {"m1", 0, 0, (57.1-10.1)/1779, 0, 0, 0, 2, 3, 0, 0};
motorDC m2 = {"m2", 0, 0, (52.9-15)/3078, 0, 0, 0, 4, 5, 0, 0};

//Servo sv3;
//motorSV m3 = {90, 0};

// Initialization rotary encoder
Encoder rotEncX(ROTENCXA, ROTENCXB);
Encoder rotEncY(ROTENCYA, ROTENCYB);

void setup() {
  Serial.begin(115200);
  Serial.println("Title:      DynaTable Arduino Program");
  Serial.println("Board:      Arduino Mega/Arduino Mega 2560");
  Serial.println("Version:    8");
  Serial.println("Date:       19/12/2019");
  Serial.println("Author(s):  C. Kos, B. Cheizoo, J. de Wolde, T. Hartsuijker");
  Serial.println();
  
  pinMode(ENABLE, INPUT);
  
	pinMode(m1.p2, OUTPUT);
	pinMode(m1.p1, OUTPUT);
 
  digitalWrite(m1.p2, LOW);
  digitalWrite(m1.p1, LOW);
  
  pinMode(m2.p2, OUTPUT);
  pinMode(m2.p1, OUTPUT);
 
  digitalWrite(m2.p2, LOW);
  digitalWrite(m2.p1, LOW);
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
      } else if (mode == false) {
        mode = true;
      } else{
        mode = mode;
      }
      changed = true;
      timer = false;
    }
  } else if (enable == false && changed == true) {
    changed = false;
    timer = false;
  } else {
    timer = false;
    
  }

  if (enable == false && changed == false && timer == true && pulseFlag == 0) {
    pulseFlag = 1;
    timer = false;
  }

  m1 = stateMachineDC(m1);
  
//  m2 = stateMachineDC(m2);

  int rREX = rotEncX.read();
  if (rREX != m1.rREPrev) {
    m1.rREPrev = rREX;
    m1.cRE += m1.add;
  }

Serial.print(enable);
Serial.print(" | ");
Serial.print(timer);
Serial.print(" | ");
Serial.print(changed);
Serial.print(" | ");
Serial.println(pulseFlag);

//  int rREY = rotEncY.read();
//  if (rREY != m2.rREPrev) {
//    m2.rREPrev = rREY;
//    m2.cRE += m2.add;
//  }
/*
  switch (m1.add) {
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

  switch (m2.add) {
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

}
