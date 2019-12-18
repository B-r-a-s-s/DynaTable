#include <Servo.h>

int posL = 100;
int posR = 76;
int total = posL + posR;

Servo sL, sR;

const int pL = 31;
const int pR = 33;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  sL.attach(pL);
  sR.attach(pR);

  sL.write(posL);
  sR.write(posR);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    int input = Serial.parseInt();
    if (input < 180 && input >= 0) {
      posL = input;
      posR = total-input;
      sL.write(posL);
      sR.write(posR);
    } else {
      Serial.println("Must be on interval [0, 179]");
    }
  }

}
