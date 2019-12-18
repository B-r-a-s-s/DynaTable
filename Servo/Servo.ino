#include <Servo.h>

Servo sL, sR;

int D = 500;

int posL = 60;
int posR = 120;
String dir = "L";
bool input = false;
int deg;

void setup() {
  // put your setup code here, to run once:
  
  sL.attach(13);
  sR.attach(12);

  Serial.begin(9600);

  sL.write(posL);
  sR.write(posR);

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("posL: ");
  Serial.print(posL);
  Serial.print(" | posR: ");
  Serial.println(posR);

  for (int i = 0; i < 30; i++) {
    if (Serial.available() > 0) {
      dir = Serial.readString();
      if (dir == "L" || dir == "R" || dir == "B") {

        for (int i = 0; i < 30; i++) {
          if (Serial.available() > 0) {
            deg = Serial.parseInt();
            if (deg < 180 && deg >= 0) {
              input = true;
              break;
            }else{
              Serial.println("Please enter value between 0 and 179.");
            }
          }

          delay(1000);
        }
      }else{
        Serial.println("Please enter L or R or B.");
      }
    }

    if (input == true) {
      break;
    }

    delay(1000);
  }

  if (dir == "L" && input == true) {
    posL = deg;
    posR = posR;
  }else if (dir == "R" && input == true) {
    posL = posL;
    posR = deg;
  }else if (dir == "B" && input == true) {
    posR = posR+posL-deg;
    posL = deg;
  }else{
    posL=posL;
    posR=posR;
  }
  input = false;

  sL.write(posL);
  sR.write(posR);

  delay(D);
	
}
