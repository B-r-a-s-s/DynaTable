// Rotational direction calculated from the outputs of the rotary encoder
/*
bool rotEncAPrev = false;
bool rotDirPrev = false;

bool rotDir (bool rotEncA, bool rotEncB) {
  bool rotDir = 0;
  
  if (rotEncA != rotEncAPrev) {
    rotDir = rotEncA^rotEncB && rotEncA^rotEncAPrev;
  }else{
    rotDir = rotDirPrev;
  }
  rotEncAPrev=rotEncA;
  rotDirPrev=rotDir;
  
  return rotDir;
  
}
*/
// The FSM used to determine the output of the motor

motorDC stateMachineDC(motorDC mTemp) {

  Serial.print(mTemp.mname);
  Serial.print(" | ");
  Serial.print(mTemp.mname);
  Serial.print(" | ");
  Serial.print(changed);
  Serial.print(" | ");
  Serial.println(pulseFlag);

/*        
  Serial.print(mTemp.mname);
  Serial.print(".limit: ");
  Serial.print(mTemp.limit);
  Serial.print(" | ");
  Serial.print(mTemp.mname);
  Serial.print(".cRE: ");
  Serial.println(mTemp.cRE);
*/  
  switch (mTemp.currentState) {
    case 0:

    if (millis() >= mTemp.halt + 100) {
      if (mode == 1 || (mode == 0 && enable == 1)) {
        mTemp.pulse = 0;

        pulseFlag = 0;
        
        mTemp.limit = (2*A*random(-(L-1)/2,(L-1)/2+1)/(L-1))/mTemp.mmptick;
        
        mTemp = path(mTemp);
        
      }else{
        mTemp.currentState = 0;
        mTemp.limit = 0;
//        mTemp.dirRef = 0;
      }
    }
    
    break;

    case 1:
    
    if (mTemp.cRE >= mTemp.limit) {
      digitalWrite(mTemp.p2, LOW);
      digitalWrite(mTemp.p1, LOW);
      
      mTemp.halt = millis();
      
      if (mode == 0 && mTemp.pulse == 0) {
        mTemp.limit = 0;
        mTemp.currentState = 5;
      } else {
        mTemp.currentState = 0;
        pulseFlag = 0;
      }
      
//      mTemp.limit = 0;
//      mTemp.dirRef = 0;
//      mTemp.add = -1;
    }else{
//      mTemp.currentState = 1;
//      mTemp.limit = mTemp.limit;
//      mTemp.dirRef = 1;
    }
    
    break;
/*
    case 2:
    
    if (mTemp.cRE <= mTemp.limit) {
      mTemp.currentState = 0;
      mTemp.limit = 0;
      mTemp.dirRef = 0;
 
      digitalWrite(DCXM2, LOW);
      digitalWrite(DCXM1, LOW);
    }else{
//      mTemp.currentState = 2;
//      mTemp.limit = 0;
//      mTemp.dirRef = -1;
    }
    
    break;
*/
    case 3:
    
    if (mTemp.cRE <= mTemp.limit) {
      digitalWrite(mTemp.p2, LOW);
      digitalWrite(mTemp.p1, LOW);
      
      mTemp.halt = millis();
      
      if (mode == 0 && mTemp.pulse == 0) {
        mTemp.limit = 0;
        mTemp.currentState = 5;
      } else {
        mTemp.currentState = 0;
        pulseFlag = 0;
      }
      
//      mTemp.limit = 0;
//      mTemp.dirRef = 0;
//      mTemp.add = -1;
    }else{
//      mTemp.currentState = 1;
//      mTemp.limit = mTemp.limit;
//      mTemp.dirRef = 1;
    }
    
    break;
/*
    case 4:
    
    if (mTemp.cRE >= mTemp.limit) {
      mTemp.currentState = 0;
      mTemp.limit = 0;
      mTemp.dirRef = 0;
 
      digitalWrite(DCXM2, LOW);
      digitalWrite(DCXM1, LOW);
    }else{
      mTemp.currentState = 4;
      mTemp.limit = 0;
      mTemp.dirRef = 1;
    }
    
    break;
*/    
    case 5:

    if (millis() >= mTemp.halt + 100) {
      mTemp.pulse = 1;
      mTemp = path(mTemp);
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
        
      } while (mTemp.limit == 90);
      
      mTemp.currentState = 1;
      
    }else{
      mTemp.currentState = 0;
      mTemp.limit = 90;
    }
    
    break;

    case 1:

    if (true) {
      mTemp.currentState = 2;
      mTemp.limit = 90;
    }else{
      mTemp.currentState = 1;
      mTemp.limit = mTemp.limit;
    }
    
    break;

    case 2:

    if (true) {
      mTemp.currentState = 0;
      mTemp.limit = 90;
    }else{
      mTemp.currentState = 2;
      mTemp.limit = mTemp.limit;
    }
    
    break;
    
  }

  return mTemp;
  
}

motorDC path(motorDC mTempP) {
  if (mTempP.limit > mTempP.cRE) {
//    mTemp.dirRef = 1;
    mTempP.add = 1;
    mTempP.currentState = 1;
    
    digitalWrite(mTempP.p2, HIGH);
    digitalWrite(mTempP.p1, LOW);
  }else if (mTempP.limit < mTempP.cRE) {
//    mTemp.dirRef = -1;
    mTempP.add = -1;
    mTempP.currentState = 3;
    
    digitalWrite(mTempP.p2, LOW);
    digitalWrite(mTempP.p1, HIGH);
  }else{
//    mTemp.dirRef = 0;
    mTempP.currentState = 0;
  }
  
  Serial.print(mTempP.mname);
  Serial.print(".limit: ");
  Serial.println(mTempP.limit);

  return mTempP;
}
