#include "booleansensor.h"

BooleanSensor::BooleanSensor(int pin, QString n, QObject *parent)
    : Sensor{n,parent},m_pin(pin)
{

}
