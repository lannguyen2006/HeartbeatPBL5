#define PulseSensor 32
#define HBPin 2

int PulseSensorSignal;
int UpperThreshold = 1860;
int LowerThreshold = 1890;
int cntHB = 0;
boolean Threshold = true;
int BPMval = 0;
bool firsttime = true;
unsigned long lastMillisGetHB = 0;
unsigned long currentMillisGetHB = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(HBPin, OUTPUT);
}

int readHeartRate()
{
  currentMillisGetHB = millis();
  if (firsttime) 
  {
    lastMillisGetHB = currentMillisGetHB;
    cntHB = 0;
    firsttime = false;
  }

  if (currentMillisGetHB - lastMillisGetHB >= 15000) 
  {
    BPMval = cntHB * 6; // Tính nhịp tim mỗi 15 giây
    lastMillisGetHB = currentMillisGetHB;
    cntHB = 0;
    firsttime = true;
  } 
  else 
  {
    PulseSensorSignal = analogRead(PulseSensor);
    //Serial.print("PulseSensorSignal: ");
    Serial.println(PulseSensorSignal);
    if (PulseSensorSignal > UpperThreshold && Threshold == true) 
    {
      cntHB++;
      Threshold = false;
      digitalWrite(HBPin, HIGH);
      //Serial.print("Count: ");
      //Serial.println(cntHB);
    }
    if (PulseSensorSignal < LowerThreshold) 
    {
      Threshold = true;
      digitalWrite(HBPin, LOW);
    }
  }
  return BPMval;
}

void loop()
{
  readHeartRate();
  delay(100);
}
