

//Motors
#define HEIGH_LIFTER_UP 2
#define HEIGH_LIFTER_DOWN 4
#define WIND_ROTATION_PIN 6
#define WIND_ROTATION_PIN_2 8

// PWM output
#define LIGHTS_SPECTRUM_PIN 3// pas cablé

//12VSwitch
#define CO2_INJECTOR_PIN 39
#define MAIN_VALVE_PIN 41
#define HUMIDIFIER_PIN 43
#define INTERNAL_FAN_PIN 19


//Relay
#define MAIN_PUMP_PIN 26
#define EXTRACTOR_PIN 25
#define LIGHTS_POWER_PIN 22
#define WIND_POWER_PIN 27
#define RELAY_5 28

//Input pins
#define WATER_LEVEL_PIN_1 14
#define WATER_LEVEL_PIN_2 16


//Analog input pins
#define PH_SENSOR_PIN 97
#define POWER_SENSE2 98
#define POWER_SENSE 99 // pas cablé
#define CO2_SENSOR_PIN 100

//virtual pins
#define HEIGH_SENSE 120
#define HUMIDTY_PIN_1 121
#define HUMIDTY_PIN_2 122
#define HUMIDTY_PIN_3 123
#define TEMP_1_PIN 125
#define TEMP_2_PIN 126
#define TEMP_3_PIN 127


#define DHT_1 18
#define DHT_2 32
#define DHT_3 34


#define NUM_IN_ANALOG_PINS 4
#define NUM_IN_PULL_PINS 2
#define NUM_OUT_PINS 13
#define NUM_CONFIG 3

#include <Wire.h>
#include <VL53L0X.h>
VL53L0X distance;
bool distSensorFound;
int distanceValue;




#include "DHT_Async.h"
#define DHT_SENSOR_TYPE DHT_TYPE_11
DHT_Async dht_1(DHT_1, DHT_SENSOR_TYPE);
DHT_Async dht_2(DHT_2, DHT_SENSOR_TYPE);
DHT_Async dht_3(DHT_3, DHT_SENSOR_TYPE);
int temperature[3];
int dhtTime[3];
int humidity[3];
bool dhtState[3]{true,true,true};
int DHT_pins[3]={DHT_1,DHT_2,DHT_3};
int DHT_humiditypins[3]={HUMIDTY_PIN_1,HUMIDTY_PIN_2,HUMIDTY_PIN_3};
int DHT_temperaturepins[3]={TEMP_2_PIN,TEMP_2_PIN,TEMP_2_PIN};


const int inputAnalogPins[NUM_IN_ANALOG_PINS]={PH_SENSOR_PIN,POWER_SENSE2,POWER_SENSE,CO2_SENSOR_PIN};
const int inputPinsPull[NUM_IN_PULL_PINS]= {WATER_LEVEL_PIN_1,WATER_LEVEL_PIN_2};
const int outputPins[NUM_OUT_PINS] = 
{HEIGH_LIFTER_UP,HEIGH_LIFTER_DOWN,
WIND_ROTATION_PIN,WIND_ROTATION_PIN_2,
INTERNAL_FAN_PIN,MAIN_PUMP_PIN,
EXTRACTOR_PIN,LIGHTS_POWER_PIN,
RELAY_5,INTERNAL_FAN_PIN,
CO2_INJECTOR_PIN,MAIN_VALVE_PIN,
HUMIDIFIER_PIN};


boolean configReceived = false;
byte configBytes[NUM_CONFIG];
unsigned long previousMillis = 0;
const long interval = 200;

static bool measure_environment() {
    static unsigned long measurement_timestamp = millis();
    
    if (millis() - measurement_timestamp > 4000ul) {
      for(int i=0;i<3;i++)
      {

        float t;
        float h;
        int res;

        
        if(i==0 && dhtState[0])
          dhtState[0]=dht_1.measure(&t, &h);

        else if(i==1 && dhtState[1])
          dhtState[1]=dht_2.measure(&t, &h);

         else if(i==2 && dhtState[2])
          dhtState[2]=dht_3.measure(&t, &h);
  
        temperature[i]=t;
        humidity[i]=h;
        }
        measurement_timestamp = millis(); 
        return true;
    }
       
    return (false);
} 
void sendValues()
{ 

  
   for(int i=0;i<3;i++)
   {
     byte hb=map(humidity[i],0,100,0,255);
     Serial.write(DHT_humiditypins[i]);
     Serial.write(hb);

     byte ht=map(temperature[i],0,100,0,255);
     Serial.write(DHT_temperaturepins[i]);
     Serial.write(ht);
   }

    
    byte c=map(distanceValue,0,2000,0,255);
    Serial.write(HEIGH_SENSE);
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

    
  measure_environment();

   
   
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    sendValues();
  }

  
      
}
