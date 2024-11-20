#define PulseSensor 32
#define HBPin 2

int PulseSensorSignal;
int UpperThreshold = 2860;
int LowerThreshold = 2845;
int cntHB = 0;
boolean Threshold = true;
int BPMval = 0;
bool firsttime = true;
unsigned long lastMillisGetHB = 0;
unsigned long currentMillisGetHB = 0;
void loop() 
{
    currentMillisGetHB = millis();
    if (firsttime) 
    {
      lastMillisGetHB = currentMillisGetHB;
      cntHB = 0;
      firsttime = false;
    }
      if (currentMillisGetHB - lastMillisGetHB >= 10000) {
        BPMval = cntHB * 6;
        lastMillisGetHB = currentMillisGetHB;
        cntHB = 0;
        firsttime = true;
      } 
    else {
      PulseSensorSignal = analogRead(PulseSensor);
      Serial.print("PulseSensorSignal: ");
      Serial.println(PulseSensorSignal);
        if (PulseSensorSignal > UpperThreshold && Threshold == true) {
          cntHB++;
          Threshold = false;
          digitalWrite(HBPin, HIGH);
          Serial.print("Count: ");
          Serial.println(cntHB);
        }
        if (PulseSensorSignal < LowerThreshold) {
          Threshold = true;
          digitalWrite(HBPin, LOW);
        }
    }
}
  void setup() 
{
  Serial.begin(115200);
  pinMode(HBPin, OUTPUT);
  
}
