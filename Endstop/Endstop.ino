const int ESP = 31;

int ESI;

int edgeFlag = false;

// TEMPORARY TIMER (proof of concept)
unsigned long edgeWait = 1500;
unsigned long edgeTime;

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
  
  Serial.begin(115200);
  Serial.println("Start-up");

}

void loop() {
  
  ESI = digitalRead(ESP);

  if (edgeFlag == true) {
    if (ESI == 1) {
      edgeFlag = false;
      edgeTime = millis(); // testing
      Serial.println("Positive detected; detection temporarily unavailable");
    } else {
    }
  } else {
  }

  if (millis() >= edgeTime + edgeWait && edgeFlag == false) { // testing
    edgeFlag = true;
    Serial.println("Detection available");
  } else {
  }

}
