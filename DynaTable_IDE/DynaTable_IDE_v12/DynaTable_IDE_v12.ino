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
  float mmptick;
  int cRE, limit, currentState, rREPrev, add, p1, p2, ESP, calCount, range, limitPrev;
  unsigned long halt, Ttarget;
  bool ESI, goBack, doPulse, switchFlag; // switchFlag true when switching from continuous to pulse mode
};

struct motorSV {
  float limit;
  int currentState;
  bool doPulse, switchFlag;
};

// Declaration of pins

const int ENABLE = 8; // Enable pin (button)

const int randPin = A0; // used for seed generation

const int ROTENCXA = 11; // Rotary encoder pins
const int ROTENCXB = 12;
const int ROTENCYA = 40;
const int ROTENCYB = 23;

const int m1p1 = 2;
const int m1p2 = 3;

const int m2p1 = 4;
const int m2p2 = 5;

const int m1ESP = 30;
const int m2ESP = 31;

// Initialization of constants

const int TpressLong = 1000; // duration button must be pressed to switch mode
const int TpressInter = 500; // cool-down between given pulses
const int TpressShort = 50; // minimal duration button must be pressed to be counted as a pulse
const int caseDelay = 100; // delay between switching cases
const int switchDelay = 1000; // delay after switching from continuous mode
const int targetTimeOut = 5000; // duration after which a motor is assumed stuck and everything is turned off

//const int range1 = 36; // maximum range (mm) (twice the amplitude) | full reach
const int range1 = 24; // maximum range (mm) (twice the amplitude) | safe to leave unattended (doesn't touch bridge edge)
const int range2 = 48; // maximum range (mm) (twice the amplitude)

const int maxMoves = 5; // Number of moves before forced calibration

const float Tservo = 500; // Servo time in ms (min. 450 ms)

const float m1ticks = (57.1-10.1)/1779; // mm/tick
const float m2ticks = (52.9-15)/3078; // mm/tick

/*
const int SV3 = ?;
const int SV4 = ?;
*/

// Initialization of variables used in the main functions and the FSM

bool mode = false; // continuous mode if true, pulse mode if false
bool enable = false; // one button to rule them all
bool enablePrev = false; // used for falling edge in pulse mode
bool changed = false;
bool enableTimer = false;
bool globalError = false; // if this is true (motor is stuck), everything is turned off until reset

unsigned long Tenable;
unsigned long Tpulse;
unsigned long Tswitch;

int rREX;
int rREY;

// Initialization of motor structures

motorDC m1 = {"m1", m1ticks, 0, 0, 0, 0, 0, m1p1, m1p2, m1ESP, maxMoves, range1, 2, 0, 0, false, false, false, false};
motorDC m2 = {"m2", m2ticks, 0, 0, 0, 0, 0, m2p1, m2p2, m2ESP, maxMoves, range2, 2, 0, 0, false, false, false, false};

motorSV m3 = {90, 0, false, false};

// Initialization rotary encoder
Encoder rotEncX(ROTENCXA, ROTENCXB);
Encoder rotEncY(ROTENCYA, ROTENCYB);

void setup() {
  Serial.begin(9600);
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
  
  checkButton();

  m1 = stateMachineDC(m1);
  
  m2 = stateMachineDC(m2);

  updateREX();
  
  updateREY();

//  m3 = stateMachineSV(m3);

//  sv3.write(m3.limit);


}
