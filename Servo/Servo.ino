#include <Servo.h>

Servo sL, sR;

int D = 2000;

int LB = 65;
int HB = 110;

int posL = 60;
int posR = 180 - posL;
String dir = "B";
bool input = false;
int deg;
int flag = 0;

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
  Serial.print(posR);
  Serial.println();
/*
  if (flag == 1) {
    posL = HB;;
    posR = LB;
    flag = 0;
  } else if (flag == 0) {
    posL = LB;
    posR = HB;
    flag = 1;
  } else {
    flag = 0;
  }
*/
  Serial.println("Type 'L', 'R' or 'B' (30 sec)");
  for (int i = 0; i < 30; i++) {
    if (Serial.available() > 0) {
      dir = Serial.readString();
      Serial.print("dir: ");
      Serial.println(dir);
      if (dir == "L" || dir == "R" || dir == "B") {

        Serial.println("Type 0 <= deg <= 179 (30 sec)");
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
