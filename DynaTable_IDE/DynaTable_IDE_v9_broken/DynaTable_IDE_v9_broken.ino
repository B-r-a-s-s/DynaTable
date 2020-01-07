#include <Servo.h>
#include <math.h>
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
  int currentState, rREPrev, add, p1, p2, ESP;
  unsigned long halt;
  bool back;
};

struct motorSV {
  String mname;
  float limit;
  int currentState, rPos;
  unsigned long halt;
  bool back;
};

// Initialization of constants

const long Tpress = 1000;

const int L = 5; // Number of levels
const int A = 1; // Maximum amplitude (mm)

const float Tservo = 500; // Servo time in ms (min. 450 ms)
const int LB = 65;
const int HB = 99;
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

bool mode = false; // Continuous mode if 'true', pulse mode if 'false'
bool enable = false; // In pulse mode, start cycle if 'true'

bool changed = false;

unsigned long tmode = 0;
bool timer = false;

String startpause = "stop";
String startpauseprev = "stop";

// Initialization of motor structures

motorDC m1 = {"m1", 0, 0, (57.1-10.1)/1779, 0, 0, 0, 2, 3, 30, 0, false}; // spindle
motorDC m2 = {"m2", 0, 0, (52.9-15)/3078, 0, 0, 0, 4, 5, 31, 0, false}; // timing belt

//Servo sv3;
motorSV m3 = {"m3", 90, 0, 90, 0, false};

// Initialization rotary encoder
Encoder rotEncX(ROTENCXA, ROTENCXB);
Encoder rotEncY(ROTENCYA, ROTENCYB);

void setup() {
  Serial.begin(115200);
  Serial.println("Title:      DynaTable Arduino Program");
  Serial.println("Board:      Arduino Mega/Arduino Mega 2560");
  Serial.println("Version:    9");
  Serial.println("Date:       18/12/2019");
  Serial.println("Author(s):  C. Kos, B. Cheizoo, J. de Wolde, T. Hartsuijker");
  Serial.println();
  Serial.println("Type 'start' to start. Type 'pause' to pause.");
  Serial.println();
  
  pinMode(ENABLE, INPUT);
  
	pinMode(m1.p2, OUTPUT); // IN2 / OUT2 + white
	pinMode(m1.p1, OUTPUT); // IN1 / OUT1 + red
  
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

  if (Serial.available() > 0) {
    startpause = Serial.readString();
    if (startpause == "start" || startpause == "pause") {
      
    } else {
      startpause = startpauseprev;
    }
    startpauseprev = startpause;
  }

  if (startpause == "start") {
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
    
//    m2 = stateMachineDC(m2);
  
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
*/    
//    m3 = stateMachineSV(m3);
  }

}
