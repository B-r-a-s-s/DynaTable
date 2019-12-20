#include <Encoder.h>
/*
y1: 52.9mm
y2: 15mm
ticks: 3078
mm/tick (Y): 0.01231319038336582196231319038337

x1: 57.1mm
x2: 10.1mm
ticks: 1779
mm/tick (X): 0.0264193367060146149522203485104‬
*/

float xFac = 1779 / (57.1 - 10.1);

int xmm = 0;
long xGoal = 0;
int xDirFlag = 0;
int xCountFlag = 0;

const int MxPos = 2; // IN1 & OUT1 & red
const int MxNeg = 3; // IN2 & OUT2 & white

const int ROTENCAX = 11; // Rotary encoder pins
const int ROTENCBX = 12;

long cREX = 0;
int rREXPrev = 0;

unsigned long T = 0;
bool start = false;

// Initialization rotary encoder
Encoder rotEncX(ROTENCAX, ROTENCBX);

void setup() {

  Serial.begin(115200);
  Serial.println("Test");

  pinMode(MxPos, OUTPUT);
  pinMode(MxNeg, OUTPUT);

  digitalWrite(MxPos, LOW);
  digitalWrite(MxNeg, LOW);

}

void loop() {

  if (Serial.available() > 0) {
    xmm = Serial.parseInt();
    xGoal = xFac * xmm;
    Serial.println(xGoal);
    if (xGoal >= cREX) {
      xDirFlag = 1;
      xCountFlag = 1;
      digitalWrite(MxPos, HIGH);
      digitalWrite(MxNeg, LOW);
    } else if (xGoal <= cREX) {
      xDirFlag = -1;
      xCountFlag = -1;
      digitalWrite(MxPos, LOW);
      digitalWrite(MxNeg, HIGH);
    } else {
      xDirFlag = 0;
      digitalWrite(MxPos, LOW);
      digitalWrite(MxNeg, LOW);
    }
  }

  int rREX = rotEncX.read();
  if (rREX != rREXPrev) {
    rREXPrev = rREX;
    cREX += xCountFlag;
    Serial.println(cREX);
  }

  xMove();

}

void xMove() {
  if (xDirFlag == 1) {
    if (cREX >= xGoal) {
      xDirFlag = 0;
      Serial.println(cREX);
      Serial.println(xGoal);
      Serial.println("Goal achieved");
      digitalWrite(MxPos, LOW);
      digitalWrite(MxNeg, LOW);
    }
  } else if (xDirFlag == -1) {
    if (cREX <= xGoal) {
      xDirFlag = 0;
      Serial.println(cREX);
      Serial.println(xGoal);
      Serial.println("Goal achieved");
      digitalWrite(MxPos, LOW);
      digitalWrite(MxNeg, LOW);
    }
  }
}
