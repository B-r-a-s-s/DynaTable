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
  Serial.print("cRE: ");
  Serial.print(mTemp.cRE);
  Serial.print(" | ");
  Serial.print("limit: ");
  Serial.print(mTemp.limit);
  Serial.print(" | ");
  Serial.print("currentState: ");
  Serial.print(mTemp.currentState);
  Serial.println();
  
  switch (mTemp.currentState) {

    case 0:

    if (millis() >= mTemp.halt + 100) {
      if (mode == true || (mode == false && enable == true || mTemp.back == true)) {
        if (mTemp.back == true && mode == false) {
          mTemp.limit = 0;
          mTemp.back = false;
        } else {
          mTemp.limit = (2*A*random(-(L-1)/2,(L-1)/2+1)/(L-1))/mTemp.mmptick;
        }
        
        if (mTemp.limit > mTemp.cRE) {
//          mTemp.dirRef = 1;
          mTemp.add = 1;
          mTemp.currentState = 1;
          
          digitalWrite(mTemp.p2, HIGH);
          digitalWrite(mTemp.p1, LOW);
        }else if (mTemp.limit < mTemp.cRE) {
//          mTemp.dirRef = -1;
          mTemp.add = -1;
          mTemp.currentState = 3;
          
          digitalWrite(mTemp.p2, LOW);
          digitalWrite(mTemp.p1, HIGH);
        }else{
//          mTemp.dirRef = 0;
          mTemp.currentState = 0;
        }
        
      }else{
        mTemp.currentState = 0;
//        mTemp.limit = 0;
//        mTemp.dirRef = 0;
      }
    }
    
    break;

    case 1:
    
    if (mTemp.cRE >= mTemp.limit) {
      mTemp.back = true;
      
      mTemp.currentState = 0;
//      mTemp.limit = 0;
//      mTemp.dirRef = 0;
//      mTemp.add = -1;
      mTemp.halt = millis();

      mTemp.add = 0; // LATER VERWIJDEREN!
        
      digitalWrite(mTemp.p2, LOW);
      digitalWrite(mTemp.p1, LOW);
    } else {
//      mTemp.currentState = 1;
//      mTemp.limit = mTemp.limit;
//      mTemp.dirRef = 1;
    }
    
    break;
/*
    case 2:
    
    if (mTemp.cRE <= mTemp.limit) {

      if (mode == 0) {
        mTemp.back = true;
      }
      
      mTemp.currentState = 0;
      mTemp.limit = 0;
        
      digitalWrite(mTemp.p2, LOW);
      digitalWrite(mTemp.p1, LOW);
    } else {
//      mTemp.currentState = 2;
//      mTemp.limit = 0;
//      mTemp.dirRef = -1;
    }
    
    break;
*/
    case 3:
    
    if (mTemp.cRE <= mTemp.limit) {
      mTemp.back = true;
      
      mTemp.currentState = 0;
//      mTemp.limit = 0;
//      mTemp.dirRef = 0;
//      mTemp.add = 1;
      mTemp.halt = millis();

      mTemp.add = 0; // LATER VERWIJDEREN!
      
      digitalWrite(mTemp.p2, LOW);
      digitalWrite(mTemp.p1, LOW);
    } else {
//      mTemp.currentState = 3;
//      mTemp.limit = mTemp.limit;
//      mTemp.dirRef = -1;
    }
    
    break;
/*
    case 4:
    
    if (mTemp.cRE >= mTemp.limit) {
      mTemp.currentState = 0;
      mTemp.limit = 0;
        
      digitalWrite(mTemp.p2, LOW);
      digitalWrite(mTemp.p1, LOW);
    } else {
      mTemp.currentState = 4;
      mTemp.limit = 0;
    }
    
    break;
*/    
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
