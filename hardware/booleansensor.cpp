#include "booleansensor.h"
#include "rasppi.h"
BooleanSensor::BooleanSensor(int pin, QString n, QObject *parent)
    : Sensor{n,parent},m_pin(pin)
{

}

float BooleanSensor::aquire()
{

    bool h=RasPi::readHigh(m_pin);

    if(h)
        return 100;
    return 0;

}
