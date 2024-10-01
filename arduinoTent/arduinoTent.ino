

//Motors
#define HEIGH_LIFTER_UP 2
#define HEIGH_LIFTER_DOWN 4
#define WIND_ROTATION_PIN 6
#define WIND_ROTATION_PIN_2 8

// PWM output
#define LIGHTS_SPECTRUM_PIN 3
#define LIGHTS_POT_PIN 7
#define INTERNAL_FAN_PIN 5

//LED
#define INTERNAL_LED_PIN 15
#define LED_ID_GREEN 0
#define LED_ID_RED 2
#define LED_ID_BLUE 5

//12VSwitch
#define CO2_INJECTOR_PIN 39
#define MAIN_VALVE_PIN 41
#define HUMIDIFIER_PIN 43

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
#define PH_SENSOR_PIN 54
#define POWER_SENSE2 55
#define POWER_SENSE 56 
#define CO2_SENSOR_PIN 57

//virtual pins
#define HEIGH_SENSE 120
#define HUMIDTY_PIN_1 110
#define HUMIDTY_PIN_2 111
#define HUMIDTY_PIN_3 112
#define TEMP_1_PIN 113
#define TEMP_2_PIN 114
#define TEMP_3_PIN 115

#define DHT_1 18
#define DHT_2 32
#define DHT_3 34

#define NUM_IN_ANALOG_PINS 3
#define NUM_IN_PULL_PINS 2
#define NUM_OUT_PINS 13
#define NUM_CONFIG 3










#include <Wire.h>
#include <VL53L0X.h>
#include <Adafruit_NeoPixel.h>
#include "DHT_Async.h"

VL53L0X distance;
bool distSensorFound=false;
int distanceValue=0;

#define NUMPIXELS 4
Adafruit_NeoPixel pixels(NUMPIXELS, INTERNAL_LED_PIN, NEO_GRB + NEO_KHZ800);

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
int DHT_temperaturepins[3]={TEMP_1_PIN,TEMP_2_PIN,TEMP_3_PIN};


const int inputAnalogPins[NUM_IN_ANALOG_PINS]={PH_SENSOR_PIN,POWER_SENSE2,POWER_SENSE};
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
byte CO2=0;









void readCO2()
{
 unsigned long pwmtime = pulseIn(CO2_SENSOR_PIN, HIGH, 400000) / 1000;
 float pulsepercent = pwmtime / 1004.0;

 if(pulsepercent>0)
    CO2 = 255 * pulsepercent;
}
void readDistance()
{
  if(distSensorFound)
    distanceValue=distance.readRangeSingleMillimeters();
    
  else
  {
    distSensorFound = (distance.init());
    distanceValue=0;
    return;
  }
    
  if(distanceValue>8000)
    distanceValue=0;
}
void printColor(byte colorId, bool s)
{
  pixels.clear(); 
  for(int i=0; i<NUMPIXELS; i++) 
  {
    switch(colorId)
    {
      case LED_ID_RED: pixels.setPixelColor(i, pixels.Color(255,0,0)); break;
      case LED_ID_GREEN: pixels.setPixelColor(i, pixels.Color(0,0,255)); break;
      case LED_ID_BLUE: pixels.setPixelColor(i, pixels.Color(0,255,0)); break;
      default: pixels.setPixelColor(i, pixels.Color(255,0,255)); break;
    }

    delay(5); 
    pixels.show();
  }
}
static bool measure_environment() {
    static unsigned long measurement_timestamp = millis();
    if (millis() - measurement_timestamp > 4000ul) {
      for(int i=0;i<3;i++)
      {
        float t;
        float h;

        
        if(i==0)
        {
          dhtState[0]=dht_1.measure(&t, &h);
          temperature[i]=t;
          humidity[i]=h;
        }

        else if(i==1 && dhtState[1])
        {
          dhtState[1]=dht_2.measure(&t, &h);
          temperature[i]=t;
          humidity[i]=h;
        }

        
         else if(i==2 && dhtState[2])
         {
            dhtState[2]=dht_3.measure(&t, &h);
            temperature[i]=t;
            humidity[i]=h;
         }
        
  

            
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

    Serial.write(CO2_SENSOR_PIN);
    Serial.write(CO2);
    
    byte c=map(distanceValue,0,2000,0,255);
    Serial.write(HEIGH_SENSE);
    Serial.write(c);
    
     
    for (int i = 0; i < NUM_IN_ANALOG_PINS; i++) 
    {  
      byte a=map(analogRead(inputAnalogPins[i]),0,1023,0,255);
      Serial.write(inputAnalogPins[i]);
     // Serial.write(50);
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
  switch(configBytes[0])
  {
    case INTERNAL_LED_PIN:printColor(configBytes[1],true);break;
    default:analogWrite(configBytes[0],configBytes[1]);break;
  }
}
void readConfig()
{
   if (Serial.available() >= NUM_CONFIG) 
  {
    for (int i = 0; i < NUM_CONFIG; i++) 
    {
      configBytes[i] = Serial.read();
    }
    if (configBytes[NUM_CONFIG-1] == '\n') 
      processConfig();
  }
}

void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  pixels.begin();
  printColor(5,false);


  pinMode(CO2_SENSOR_PIN, INPUT);
  distance.setTimeout(500);
  if(distance.init())
  {
      distSensorFound=true;
      distance.startContinuous();
  }

 
  for (int i = 0; i < NUM_IN_PULL_PINS; i++)
    pinMode(inputPinsPull[i], INPUT_PULLUP);
  
   for (int i = 0; i < NUM_IN_ANALOG_PINS; i++) 
    pinMode(inputAnalogPins[i], INPUT);
  
  for (int i = 0; i < NUM_OUT_PINS; i++) 
    pinMode(outputPins[i], OUTPUT);
  
}

void loop() {
  readConfig();

  readDistance();
  measure_environment();
  readCO2();

   
   
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    sendValues();
  }  
}
