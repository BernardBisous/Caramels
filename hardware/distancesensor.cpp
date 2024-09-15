#include "distancesensor.h"

DistanceSensor::DistanceSensor(int t, QString name, QObject *parent)
    : AnalogSensor{t,name,parent}
{
}
