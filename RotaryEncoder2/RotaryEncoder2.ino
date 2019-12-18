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
const int M1 = 2;
const int M2 = 3;

int dirRef = 0;

const int ROTENCAX = 11; // Rotary encoder pins
const int ROTENCBX = 12;

long cREX = 0;
int rREXPrev = 0;

unsigned long T = 0;
bool start = false;

// Initialization rotary encoder
Encoder rotEncX(ROTENCAX, ROTENCBX);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Test");

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:`
  if (Serial.available() > 0) {
    String str = Serial.readString();
    if (str == "start") {
      digitalWrite(M1, HIGH);
      T = millis();
      dirRef = 1;
      start = true;
      Serial.println("high");
    }
  }

  int rREX = rotEncX.read();
  if (rREX != rREXPrev) {
    rREXPrev = rREX;
    cREX = cREX + dirRef;
    Serial.println(cREX);
  }

  if (start == true) {
    if (millis() >= T + 2500) {
      digitalWrite(M1, LOW);
      dirRef = 0;
      Serial.println("low");
      start = false;
    }
  }

}
