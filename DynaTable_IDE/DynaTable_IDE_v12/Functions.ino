// Functions used in the main code and the FSM's

motorDC path(motorDC mTempP) {
  
  if (mTempP.limit > mTempP.cRE) {
    mTempP.add = 1;
    mTempP.currentState = 1;
    digitalWrite(mTempP.p1, HIGH);
  } else if (mTempP.limit < mTempP.cRE) {
    mTempP.add = -1;
    mTempP.currentState = 2;
    digitalWrite(mTempP.p2, HIGH);
  } else {
    mTempP.currentState = 0;
  }
  
  return mTempP;
  
}

void checkButton() {
  
  if (enablePrev != enable) {
    enablePrev = enable;
//    Serial.println(enable);
  }
  
  enable = digitalRead(ENABLE);
  
  if (enable == true && changed == false) {
    if (enableTimer == false) {
      Tenable = millis();
      enableTimer = true;
    }
    if (millis() >= Tenable + TpressLong) {
      if (mode == true) {
        Serial.println("Switched to pulse mode");
        mode = false;
        m1.switchFlag = true;
        m2.switchFlag = true;
//        m3.switchFlag = true; // NEEDS TO BE FIXED LATER - DON'T FORGET (can't be enabled before properly switched to false in corresponding code)
        Tswitch = millis();
      } else if (mode == false) {
        Serial.println("Switched to continuous mode");
        mode = true;
      } else{
      }
      changed = true;
      enableTimer = false;
    }
  } else if (enable == false && changed == true) {
    changed = false;
    enableTimer = false;
  } else if (enable == false && changed == false && mode == false && enablePrev == true && millis() >= Tpulse + TpressInter && millis() >= Tenable + TpressShort && m1.switchFlag == false && m2.switchFlag == false && m3.switchFlag == false) {
    Serial.println("Pulse given");
    m1.doPulse = true;
    m2.doPulse = true;
    m3.doPulse = true;
    Tpulse = millis();
  } else {
    enableTimer = false;
  }
}

void updateREX() {
  rREX = rotEncX.read();
  if (rREX != m1.rREPrev) {
//    Serial.println(rREX);
    m1.rREPrev = rREX;
    m1.cRE += m1.add;
  }
}

void updateREY() {
  rREY = rotEncY.read();
  if (rREY != m2.rREPrev) {
//    Serial.println(rREY);
    m2.rREPrev = rREY;
    m2.cRE += m2.add;
  }
}
