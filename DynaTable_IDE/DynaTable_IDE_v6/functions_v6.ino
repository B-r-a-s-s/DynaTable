// Rotational direction calculated from the outputs of the rotary encoder

//bool rotEncAPrev = false;
//bool rotDirPrev = false;
//
//bool rotDir (bool rotEncA, bool rotEncB) {
//  bool rotDir = 0;
//  
//  if (rotEncA != rotEncAPrev) {
//    rotDir = rotEncA^rotEncB && rotEncA^rotEncAPrev;
//  }else{
//    rotDir = rotDirPrev;
//  }
//  rotEncAPrev=rotEncA;
//  rotDirPrev=rotDir;
//  
//  return rotDir;
//  
//}

// The FSM used to determine the output of the motor

motorDC stateMachineDC(motorDC mTemp) {
  
  switch (mTemp.currentState) {
    case 0:
    
    if (mode == 1 || (mode == 0 && enable == 1)) {
      do {
        
        mTemp.limit = 2*A*random(-(L-1)/2,(L-1)/2+1)/(L-1);
//        mTemp.limit = 0.01;
        
      } while (mTemp.limit == 0);
      
      if (mTemp.limit > 0) {
        mTemp.dirRef = 1;
        mTemp.currentState = 1;
      }else if (mTemp.limit < 0) {
        mTemp.dirRef = -1;
        mTemp.currentState = 3;
      }else{
        mTemp.dirRef = 0;
        mTemp.currentState = 0;
      }
      
    }else{
      mTemp.currentState = 0;
      mTemp.limit = 0;
      mTemp.dirRef = 0;
    }
    
    break;

    case 1:
    
    if (mTemp.z >= mTemp.limit) {
      mTemp.currentState = 2;
      mTemp.limit = 0;
      mTemp.dirRef = -1;
    }else{
      mTemp.currentState = 1;
      mTemp.limit = mTemp.limit;
      mTemp.dirRef = 1;
    }
    
    break;

    case 2:
    
    if (mTemp.z <= mTemp.limit) {
      mTemp.currentState = 0;
      mTemp.limit = 0;
      mTemp.dirRef = 0;
    }else{
      mTemp.currentState = 2;
      mTemp.limit = 0;
      mTemp.dirRef = -1;
    }
    
    break;

    case 3:
    
    if (mTemp.z <= mTemp.limit) {
      mTemp.currentState = 4;
      mTemp.limit = 0;
      mTemp.dirRef = 1;
    }else{
      mTemp.currentState = 3;
      mTemp.limit = mTemp.limit;
      mTemp.dirRef = -1;
    }
    
    break;

    case 4:
    
    if (mTemp.z >= mTemp.limit) {
      mTemp.currentState = 0;
      mTemp.limit = 0;
      mTemp.dirRef = 0;
    }else{
      mTemp.currentState = 4;
      mTemp.limit = 0;
      mTemp.dirRef = 1;
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
        
        mTemp.limit = random(30, 150);
        
      } while (mTemp.limit == 90);
      
      mTemp.currentState = 1;

      counter++;
      
    }else{
      mTemp.currentState = 0;
      mTemp.limit = 90;
      counter = 0;
    }
    
    break;

    case 1:

    if (counter >= (T/D)) {
      mTemp.currentState = 2;
      mTemp.limit = 90;
      counter = 0;
    }else{
      mTemp.currentState = 1;
      mTemp.limit = mTemp.limit;
      counter++;
    }
    
    break;

    case 2:

    if (counter >= (T/D)) {
      mTemp.currentState = 0;
      mTemp.limit = 90;
      counter = 0;
    }else{
      mTemp.currentState = 2;
      mTemp.limit = mTemp.limit;
      counter++;
    }
    
    break;
    
  }

  return mTemp;
  
}
