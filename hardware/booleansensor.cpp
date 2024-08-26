#include "booleansensor.h"
#include <QRandomGenerator>
BooleanSensor::BooleanSensor(int pin, QString n, QObject *parent)
    : Sensor{n,parent},m_pin(pin)
{

}

float BooleanSensor::aquire()
{

    float v=QRandomGenerator::global()->generate() % 2;
    v=v*100;
    return v;
}
