

#define NUM_IN_ANALOG_PINS 7
#define NUM_IN_PULL_PINS 2
#define NUM_OUT_PINS 23
#define NUM_CONFIG 3

#include <Wire.h>

#include <VL53L0X.h>
VL53L0X distance;
const int distancePin = 62;
bool distSensorFound;
int distanceValue;

#include <DHT11.h>
const int temperaturePin= 63; 
const int humidityPin =64;
int temperature;
int humidity;

DHT11 dht11(2);

const int inputAnalogPins[NUM_IN_ANALOG_PINS] = {A0,A1,A2,A3,A4,A5,A6};
const int inputPinsPull[NUM_IN_PULL_PINS] = {47,48};
const int outputPins[NUM_OUT_PINS] = {3,4,5,6,7,8,10,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39};

boolean configReceived = false;
byte configBytes[NUM_CONFIG];
unsigned long previousMillis = 0;
const long interval = 200;

void sendValues()
{ 
     byte hb=map(humidity,0,100,0,255);
     Serial.write(humidityPin);
     Serial.write(hb);

     byte ht=map(temperature,0,100,0,255);
     Serial.write(temperaturePin);
     Serial.write(ht);
    
    byte c=map(distanceValue,0,2000,0,255);
    Serial.write(distancePin);
    Serial.write(c);
     
    for (int i = 0; i < NUM_IN_ANALOG_PINS; i++) 
    {  
      byte a=map(analogRead(inputAnalogPins[i]),0,1023,0,255);
      Serial.write(inputAnalogPins[i]);
      Serial.write(a);
    }
    

      for (int i = 0; i < NUM_IN_PULL_PINS; i++) 
      {  
        Serial.write(inputPinsPull[i]);
        if(digitalRead(inputPinsPull[i]))
          Serial.write(255);
        else
          Serial.write(0);
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
  Wire.begin();


  //dht11.setDelay(100);   
  if(distance.init())
  {
      distSensorFound=true;
      distance.setTimeout(100);
      distance.startContinuous();
  }
  else
  {
    distSensorFound=false;
  }
 
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


  if(distSensorFound)
    distanceValue=distance.readRangeSingleMillimeters();
  else
    distanceValue=0;
    
  if(distanceValue>2000)
    distanceValue=0;

  
  dht11.readTemperatureHumidity(temperature, humidity);

   
   
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    sendValues();
  }

  
      
}
