const int ESP = 30;

int ESI_Prev;
int ESI;

int edgeFlag = false;

unsigned long edgeWait = 1000;
unsigned long edgeTime;

int checkFlag = false;

// TEMPORARY TIMER (proof of concept)
unsigned long checkLimit = 3000;
unsigned long checkTimer = 0;

/*
 * ENDSTOP PIN WIRING
 * 
 * Low to ground
 * Mid to nothing, maybe ground for completeness' sake
 * High to input pin
 */

void setup() {
  pinMode(ESP, INPUT);
  pinMode(ESP, INPUT_PULLUP);
  
  ESI_Prev = digitalRead(ESP);
  
  Serial.begin(115200);
  Serial.println("Start-up");

}

void loop() {
  
  ESI = digitalRead(ESP);

  if (checkFlag == true) {
    if (edgeFlag == true) {
      if (millis() >= edgeTime + edgeWait) {
        edgeFlag = false;
        checkFlag = false;
        checkTimer = millis();
        Serial.println("Detection temporarily unavailable");
      }
    } else if (ESI == 0 && ESI_Prev == 1) {
      edgeTime = millis();
      edgeFlag = true;
      Serial.println("Falling edge detected");
    } else {
    }
  } else if (millis() >= checkTimer + checkLimit && checkFlag == false) {
    checkFlag = true;
    Serial.println("Detection available");
  } else {
  }
  
  if (ESI != ESI_Prev) {
    ESI_Prev = ESI;
  } else {
  }

}
