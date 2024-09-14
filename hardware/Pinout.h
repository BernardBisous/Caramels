#ifndef PINOUT_H
#define PINOUT_H


#define NO_PIN -1

//PWM
#define LIGHTS_SPECTRUM_PIN 2
#define HEIGH_LIFTER_PWM 3
#define HEIGH_LIFTER_DIR 4
#define CO2_INJECTOR_PIN 5
#define WIND_ROTATION_PIN 6
#define INTERNAL_FAN_PIN 7

//Digital
#define CHEM_PUMP_PH_PLUS_PIN 24
#define CHEM_MIX_PH_PLUS_PIN 25
#define CHEM_PUMP_PH_MINUS_PIN 26
#define CHEM_MIX_PH_MINUS_PIN 27
#define CHEM_MIX_1_PIN 28
#define CHEM_PUMP_1_PIN 29
#define CHEM_MIX_2_PIN 30
#define CHEM_PUMP_2_PIN 31
#define CHEM_MIX_3_PIN 32
#define CHEM_PUMP_3_PIN 33
#define CHEM_MIX_4_PIN 34
#define CHEM_PUMP_4_PIN 35
#define CHEM_MIX_5_PIN 36
#define CHEM_PUMP_5_PIN 337
#define CHEM_MIX_6_PIN 38
#define CHEM_PUMP_6_PIN 39

//Relays
#define LIGHTS_POWER_PIN 40
#define MAIN_PUMP_PIN 8
#define MAIN_VALVE_PIN 10
#define HUMIDIFIER_PIN 43
#define WIND_POWER_PIN 44
#define EXTRACTOR_PIN 45

//Input pins
#define WATER_LEVEL_PIN_1 48
#define WATER_LEVEL_PIN_2 47

// i2C virtual pins
#define HEIGH_SENSE 200

//Analog pins
#define HUMIDTY_PIN 56
#define TEMP_4_PIN 57
#define CO2_SENSOR_PIN 58
#define PH_SENSOR_PIN 55
#define TEMP_1_PIN 54
#define TEMP_2_PIN 59
#define TEMP_3_PIN 59

enum {LIGHTS_DAY=0,

      LIGHTS_SLEEP=1,
      LIGHTS_SPECTRUM=2,
      TEMPERATURE_AIR=3,
      HUMIDITY_AIR=4,
      TEMPERATURE_WATER=5,
      LIGHT_HEIGH=6,
      CO2_LEVEL=7,
      WIND_LEVEL=8,
      WIND_ROTATION=9,
      PH_LEVEL=10,
      DRY_DELAY=11,
      WET_DELAY=12,
      CHEMICAL_1=13,
      CHEMICAL_2,
      CHEMICAL_3,
      CHEMICAL_4,
      CHEMICAL_5,
      CHEMICAL_6,
      EVENTS=20

};

#endif // PINOUT_H
