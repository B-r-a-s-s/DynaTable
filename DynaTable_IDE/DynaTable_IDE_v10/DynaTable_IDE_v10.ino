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

/*
 * Endstop wiring
 * 
 * Low to ground
 * Mid to nothing, maybe ground for completeness' sake
 * High to input pin
 */

// Declaration of structures

struct motorDC {
  String mname;
  float cRE, limit, mmptick;
  int currentState, rREPrev, add, p1, p2, pulse, ESP, ESI, calCount, amp;
  unsigned long halt;
};

struct motorSV {
  float limit;
  int currentState;
};

// Initialization of constants

const long Tpress = 1000;

const int Amp1 = 15; // Maximum amplitude (mm)
const int Amp2 = 15;

const float Tservo = 500; // Servo time in ms (min. 450 ms)

const int maxMoves = 2; // Number of moves before forced calibration

const float m1ticks = (57.1-10.1)/1779; // mm/tick
const float m2ticks = (52.9-15)/3078; // mm/tick

const int wait = 100;

/*
const int SV3 = ?;
const int SV4 = ?;
*/

// Declaration of pins

const int ENABLE = 8; // Enable pin (button)

const int randPin = A0; // used for seed generation

const int ROTENCXA = 11; // Rotary encoder pins
const int ROTENCXB = 12;
const int ROTENCYA = 22;
const int ROTENCYB = 23;

const int m1p1 = 2;
const int m1p2 = 3;

const int m2p1 = 4;
const int m2p2 = 5;

const int m1ESP = 30;
const int m2ESP = 31;

// Initialization of variables used in the main functions and the FSM

bool mode = 0; // Continuous mode if 'true', pulse mode if 'false'
bool enable = 0; // In pulse mode, start cycle if 'true'

bool changed = false;

unsigned long tmode = 0;
bool timer = false;
int pulseFlag = false;

// Initialization of motor structures

motorDC m1 = {"m1", 0, 0, m1ticks, 0, 0, 0, m1p1, m1p2, false, m1ESP, false, maxMoves, Amp1, 0};
motorDC m2 = {"m2", 0, 0, m2ticks, 0, 0, 0, m2p1, m2p2, false, m2ESP, false, maxMoves, Amp2, 0};

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
  
  pinMode(m1ESP, INPUT);
  pinMode(m1ESP, INPUT_PULLUP);
  
  pinMode(m2ESP, INPUT);
  pinMode(m2ESP, INPUT_PULLUP);
  
  pinMode(m1.p1, OUTPUT); // IN2 / OUT2 + white
  pinMode(m1.p2, OUTPUT); // IN1 / OUT1 + red
 
  digitalWrite(m1.p1, LOW);
  digitalWrite(m1.p2, LOW);
  
  pinMode(m2.p1, OUTPUT);
  pinMode(m2.p2, OUTPUT);
 
  digitalWrite(m2.p1, LOW);
  digitalWrite(m2.p2, LOW);
/*
  sv3.attach(SV3);
  sv3.write(90);
*/

  randomSeed(analogRead(randPin));

}

void loop() {

//  Serial.print("m1 cRE: ");
//  Serial.println(m1.cRE);

//  Serial.print(" | Moves: ");
//  Serial.println(m1.calCount);

//  Serial.print(" | ESI: ");
//  Serial.println(m1.ESI);

//  Serial.print(" | State: ");
//  Serial.println(m2.currentState);

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

  if (enable == false && changed == false && timer == true && pulseFlag == false) {
    pulseFlag = true;
    timer = false;
  }

//  Serial.print(enable);
//  Serial.print(" | ");
//  Serial.print(timer);
//  Serial.print(" | ");
//  Serial.print(changed);
//  Serial.print(" | ");
//  Serial.println(pulseFlag);

  m1 = stateMachineDC(m1);
  
  m2 = stateMachineDC(m2);

  int rREX = rotEncX.read();
  if (rREX != m1.rREPrev) {
    m1.rREPrev = rREX;
    m1.cRE += m1.add;
  }

  int rREY = rotEncY.read();
  if (rREY != m2.rREPrev) {
    m2.rREPrev = rREY;
    m2.cRE += m2.add;
  }

//  m3 = stateMachineSV(m3);

//  sv3.write(m3.limit);


}
