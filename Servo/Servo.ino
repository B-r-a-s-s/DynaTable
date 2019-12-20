#include <Servo.h>

Servo sL, sR;

int D = 2000;

int LB = 65;
int HB = 99;

int posL = LB;
int posR = HB;
String dir = "B";
bool input = false;
int deg;
int flag = 0;

bool autoFlag = false;

void setup() {
  // put your setup code here, to run once:
  
  sL.attach(13);
  sR.attach(11);

  Serial.begin(9600);

  sL.write(posL);
  sR.write(posR);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);

  Serial.print("posL: ");
  Serial.print(posL);
  Serial.print(" | posR: ");
  Serial.print(posR);
  Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:

  if (autoFlag == true) {
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
  
    Serial.print("posL: ");
    Serial.print(posL);
    Serial.print(" | posR: ");
    Serial.print(posR);
    Serial.println();
    
  } else if (autoFlag == false) {
    Serial.print("dir: ");
    while (true) {
      if (Serial.available() > 0) {
        dir = Serial.readString();
        Serial.print(dir);
        if (dir == "L" || dir == "R" || dir == "B") {
  
          Serial.print(" | ");
          Serial.print("deg: ");
          while (true) {
            if (Serial.available() > 0) {
              deg = Serial.parseInt();
              Serial.print(deg);
              Serial.print(" (");
              Serial.print(HB+LB-deg);
              Serial.println(")");
              if (deg < 180 && deg >= 0) {
                input = true;
                break;
              }else{
                Serial.println();
                Serial.println("Please enter value between 0 and 179.");
              }
            }
  
            delay(100);
          }
        }else{
          Serial.println();
          Serial.println("Please enter L or R or B.");
        }
      }
  
      if (input == true) {
        break;
      }
  
      delay(100);
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
  } else {
    
  }

  sL.write(posL);
  sR.write(posR);

  delay(D);
	
}
