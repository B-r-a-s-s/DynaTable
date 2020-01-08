// The FSM used to determine the output of the servo motors

motorSV stateMachineSV(motorSV mTemp) {
  
  switch (mTemp.currentState) {
    
    case 0:
    
    if (mode == true || (mode == false && mTemp.doPulse == true)) {
      do {
        
        mTemp.limit = random(62, 112);
        
      } while (mTemp.limit == 90); // GEEN WHILE LOOPS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      
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
