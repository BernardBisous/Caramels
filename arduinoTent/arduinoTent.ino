#define NUM_IN_ANALOG_PINS 6
#define NUM_IN_PULL_PINS 6
#define NUM_OUT_PINS 4
#define NUM_CONFIG 3

const int inputAnalogPins[NUM_IN_ANALOG_PINS] = {A0, A1, A2, A3, A4,A5};
const int inputPinsPull[NUM_IN_PULL_PINS] = {30, 31, 32, 33, 34,35};
const int outputPins[NUM_OUT_PINS] = {8, 10, 11, 12};


boolean configReceived = false;
byte configBytes[NUM_CONFIG];
unsigned long previousMillis = 0;
const long interval = 200;

void sendValues()
{
     // Serial.print("Values: ");
      for (int i = 0; i < NUM_IN_ANALOG_PINS; i++) 
      {  
      
        byte a=map(analogRead(inputAnalogPins[i]),0,1023,0,255);
      //  Serial.print(analogRead(inputAnalogPins[i]));
      //  Serial.print(" ");
      //  Serial.print(a);
      //   Serial.print(" ; ");
       Serial.write(inputAnalogPins[i]);
       Serial.write(a);
      }

      for (int i = 0; i < NUM_IN_PULL_PINS; i++) 
      {  
        byte a=digitalRead(inputPinsPull[i]);
        Serial.write(inputPinsPull[i]);
        Serial.write(a);
      }
     
    //  Serial.print('\n');
     Serial.write('\n');
}
void processConfig()
{
   analogWrite(configBytes[0],configBytes[1]);
}


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_IN_PULL_PINS; i++) {
    pinMode(inputPinsPull[i], INPUT_PULLUP);
  }

   for (int i = 0; i < NUM_IN_ANALOG_PINS; i++) {
    pinMode(inputAnalogPins[i], INPUT);
  }

  for (int i = 0; i < NUM_OUT_PINS; i++) {
    pinMode(outputPins[i], OUTPUT);
  }
}

void loop() {
  if (Serial.available() >= NUM_CONFIG) 
  {
    for (int i = 0; i < NUM_CONFIG; i++) 
    {
      configBytes[i] = Serial.read();
    }
    if (configBytes[NUM_CONFIG-1] == '\n') 
      processConfig();
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    sendValues();
  }
}
