// The FSM used to determine the output of the motor

motorDC stateMachineDC(motorDC mTemp) {
  
//  Serial.print(mTemp.mname);
//  Serial.print(" | limit: ");
//  Serial.print(mTemp.limit);
//  Serial.print(" | cRE: ");
//  Serial.println(mTemp.cRE);
  
  switch (mTemp.currentState) {
    
    case 0: // assign new target

    if (millis() >= mTemp.halt + wait) {
      if (mode == 1 || (mode == 0 && enable == 1)) {
        if (mTemp.calCount >= maxMoves) {
          mTemp.currentState = 4;
          mTemp.calCount = 0;
          digitalWrite(mTemp.p1, HIGH);
          digitalWrite(mTemp.p2, LOW);
        } else {
          mTemp.calCount += 1;
          mTemp.limit = random(-mTemp.Amp/mTemp.mmptick,1+mTemp.Amp/mTemp.mmptick);
          mTemp = path(mTemp);
        }
        mTemp.pulse = false;
        pulseFlag = false;
      } else {
      }
    } else {
    }
    
    break;

    case 1: // move in positive direction
    
    if (mTemp.cRE >= mTemp.limit) {
      digitalWrite(mTemp.p1, LOW);
      digitalWrite(mTemp.p2, LOW);
      mTemp.halt = millis();
      if (mode == 0 && mTemp.pulse == false) {
        mTemp.currentState = 3;
      } else {
        mTemp.currentState = 0;
        pulseFlag = false;
      }
    }
    
    break;
    
    case 2: // move in negative direction
    
    if (mTemp.cRE <= mTemp.limit) {
      digitalWrite(mTemp.p1, LOW);
      digitalWrite(mTemp.p2, LOW);
      mTemp.halt = millis();
      if (mode == 0 && mTemp.pulse == false) {
        mTemp.currentState = 3;
      } else {
        mTemp.currentState = 0;
        pulseFlag = false;
      }
    }else{
    }
    
    break;

    case 3: // wait and go back to 0

    if (millis() >= mTemp.halt + wait) {
      mTemp.limit = 0;
      mTemp.pulse = true;
      mTemp = path(mTemp);
    }
    
    break;

    case 4: // calibrate

    mTemp.ESI = digitalRead(mTemp.ESP);

    if (mTemp.ESI == true) {
      digitalWrite(mTemp.p1, LOW);
      digitalWrite(mTemp.p2, LOW);
      mTemp.cRE = mTemp.Amp/mTemp.mmptick; // CHECK IF ADDITION IS AWAY FROM ENDSTOP FOR BOTH MOTORS
      mTemp.halt = millis();
      mTemp.ESI = false;
      Serial.println("Calibrated");
      if (mode == false) {
        mTemp.currentState = 3;
      } else if (mode == true) {
        mTemp.currentState = 0;
        pulseFlag = false;
      }
    }
    
    break;
      
  }

  return mTemp;
  
}

motorSV stateMachineSV(motorSV mTemp) {
  
  switch (mTemp.currentState) {
    case 0:
    
    if (mode == true || (mode == false && enable == true)) {
      do {
        
        mTemp.limit = random(62, 112);
        
      } while (mTemp.limit == 90); // GEEN WHILE LOOPS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      
      mTemp.currentState = 1;
      
    } else {
      mTemp.currentState = 0;
      mTemp.limit = 90;
    }
    
    break;

    case 1:

    if (true) {
      mTemp.currentState = 2;
      mTemp.limit = 90;
    } else {
      mTemp.currentState = 1;
      mTemp.limit = mTemp.limit;
    }
    
    break;

    case 2:

    if (true) {
      mTemp.currentState = 0;
      mTemp.limit = 90;
    } else {
      mTemp.currentState = 2;
      mTemp.limit = mTemp.limit;
    }
    
    break;
    
  }

  return mTemp;
  
}

motorDC path(motorDC mTempP) {
  if (mTempP.limit > mTempP.cRE) {
    mTempP.add = 1;
    mTempP.currentState = 1;
    digitalWrite(mTempP.p1, HIGH);
    digitalWrite(mTempP.p2, LOW);
  } else if (mTempP.limit < mTempP.cRE) {
    mTempP.add = -1;
    mTempP.currentState = 2;
    digitalWrite(mTempP.p1, LOW);
    digitalWrite(mTempP.p2, HIGH);
  } else {
    mTempP.currentState = 0;
  }
  
//  Serial.print(mTempP.mname);
//  Serial.print(".limit: ");
//  Serial.println(mTempP.limit);

  return mTempP;
}
