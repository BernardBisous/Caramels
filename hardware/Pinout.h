#ifndef PINOUT_H
#define PINOUT_H


#define NO_PIN -1




//Cablage arduino
//#define DHT_1 18
//#define DHT_2 32
//#define DHT_3 34

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

//Relays
#define MAIN_PUMP_PIN 26
#define EXTRACTOR_PIN 25
#define LIGHTS_POWER_PIN 22
#define WIND_POWER_PIN 27
#define RELAY_5 28

//12VSwitch

#define CO2_INJECTOR_PIN 39
#define MAIN_VALVE_PIN 41
#define HUMIDIFIER_PIN 43

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


//Injectors
#define CHEM_PUMP_PH_PLUS_PIN 45
#define CHEM_PUMP_PH_MINUS_PIN 47//pas cablé
#define CHEM_MIX_PH_PLUS_PIN NO_PIN
#define CHEM_MIX_PH_MINUS_PIN NO_PIN
#define CHEM_MIX_1_PIN NO_PIN
#define CHEM_PUMP_1_PIN NO_PIN
#define CHEM_MIX_2_PIN NO_PIN
#define CHEM_PUMP_2_PIN NO_PIN
#define CHEM_MIX_3_PIN NO_PIN
#define CHEM_PUMP_3_PIN NO_PIN
#define CHEM_MIX_4_PIN NO_PIN
#define CHEM_PUMP_4_PIN NO_PIN
#define CHEM_MIX_5_PIN NO_PIN
#define CHEM_PUMP_5_PIN NO_PIN
#define CHEM_MIX_6_PIN NO_PIN
#define CHEM_PUMP_6_PIN NO_PIN




enum {LIGHTS_DAY=0,
      LIGHTS_SLEEP=1,
      LIGHTS_POWER=2,
      TEMPERATURE_AIR=4,
      HUMIDITY_AIR=4,
      TEMPERATURE_WATER=5,
      LIGHT_HEIGH=6,
      CO2_LEVEL=7,
      WIND_LEVEL=8,
      WIND_ROTATION=9,
      PH_LEVEL=10,
      DRY_DELAY=11,
      WET_DELAY=12,
      LIGHTS_SPECTRUM=13,
      EVENTS=19,
      CHEMICAL_1=20,
      CHEMICAL_2,
      CHEMICAL_3,
      CHEMICAL_4,
      CHEMICAL_5,
      CHEMICAL_6,


};

#endif // PINOUT_H
