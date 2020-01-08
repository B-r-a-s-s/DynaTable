// The FSM used to determine the output of the DC motors

motorDC stateMachineDC(motorDC mTemp) {
  
  switch (mTemp.currentState) {
    
    case 0: // assign new target

    if (millis() >= mTemp.halt + caseDelay) {
      if (mode == true || (mode == false && mTemp.doPulse == true)) {
        if (mTemp.calCount >= maxMoves) {
          mTemp.currentState = 4;
          mTemp.calCount = 0;
          digitalWrite(mTemp.p1, HIGH);
        } else {
          mTemp.calCount += 1;
//          mTemp.limit = -mTemp.range / 2 / mTemp.mmptick; // used to test outer limit (opposite from endstops)
          mTemp.limit = random(-mTemp.range / 2 / mTemp.mmptick * 0.9, 1 + mTemp.range / 2 / mTemp.mmptick); // Note: only goes 95% of full range towards the endstop unless calibrating
          mTemp = path(mTemp);
        }
        mTemp.goBack = false;
        mTemp.doPulse = false;
      } else if (mTemp.switchFlag == true && millis() >= Tswitch + switchDelay - caseDelay && millis() >= Tswitch + 100) {
        mTemp.switchFlag = false;
        mTemp.currentState = 3;
        mTemp.calCount += 1;
      } else {
      }
      mTemp.Ttarget = millis();
    } else {
    }
    
    break;

    case 1: // move in positive direction

    if (mTemp.cRE >= mTemp.limit || mTemp.switchFlag == true) {
      digitalWrite(mTemp.p1, LOW);
      mTemp.halt = millis();
      if (mode == false && mTemp.goBack == false) {
        mTemp.currentState = 3;
      } else {
        mTemp.currentState = 0;
      }
    } else if (millis() >= mTemp.Ttarget + targetTimeOut) {
      digitalWrite(mTemp.p1, LOW);
      mTemp.calCount = maxMoves;
      mTemp.currentState = 0;
      mTemp.halt = millis();
    } else if (globalError == true) {
      mTemp.currentState = 5;
    } else {
    }
    
    break;
    
    case 2: // move in negative direction

    if (mTemp.cRE <= mTemp.limit || mTemp.switchFlag == true) {
      digitalWrite(mTemp.p2, LOW);
      mTemp.halt = millis();
      if (mode == false && mTemp.goBack == false) {
        mTemp.currentState = 3;
      } else {
        mTemp.currentState = 0;
      }
    } else if (millis() >= mTemp.Ttarget + targetTimeOut) {
      digitalWrite(mTemp.p2, LOW);
      mTemp.calCount = maxMoves;
      mTemp.currentState = 0;
      mTemp.halt = millis();
    } else if (globalError == true) {
      mTemp.currentState = 5;
    } else {
    }
    
    break;

    case 3: // wait and go back to 0

    if (millis() >= mTemp.halt + caseDelay) {
      mTemp.limit = 0;
      mTemp.goBack = true;
      mTemp = path(mTemp);
//      Serial.print(mTemp.mname);
//      Serial.println(" reached case 3");
    } else if (globalError == true) {
      mTemp.currentState = 5;
    } else {
    }
    
    break;

    case 4: // calibrate

    mTemp.ESI = digitalRead(mTemp.ESP);

    if (mTemp.ESI == true) {
      digitalWrite(mTemp.p1, LOW);
      mTemp.cRE = 1 + mTemp.range / 2 /mTemp.mmptick;
      mTemp.halt = millis();
      mTemp.ESI = false;
//      Serial.println("Calibrated");
      if (mode == false) {
        mTemp.currentState = 3;
      } else if (mode == true) {
        mTemp.currentState = 0;
      }
    } else if (mTemp.switchFlag == true) {
      digitalWrite(mTemp.p1, LOW);
      mTemp.halt = millis();
      mTemp.currentState = 0;
      mTemp.calCount = maxMoves;
    } else if (millis() >= mTemp.Ttarget + targetTimeOut) {
      globalError = true;
      Serial.println("Fatal error");
      mTemp.currentState = 5;
    } else if (globalError == true) {
      mTemp.currentState = 5;
    } else {
    }
    
    break;

    case 5: // global error, no way out, all motors turn off
    
    digitalWrite(mTemp.p1, LOW);
    digitalWrite(mTemp.p2, LOW);

    break;
    
  }

  return mTemp;
  
}
