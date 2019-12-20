const int ESP = 30;

int ESI_Prev;
int ESI;

void setup() {
  pinMode(ESP, INPUT);
  ESI_Prev = digitalRead(ESP);
  
}

void loop() {
  ESI = digitalRead(ESP);

  if (ESI != ESI_Prev) {
    Serial.println(ESI);
    ESI_Prev = ESI;
  } else {
    
  }
  
}
