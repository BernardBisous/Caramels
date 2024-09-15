#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#include "hardware/analogsensor.h"


#include <QObject>

class DistanceSensor : public AnalogSensor
{
    Q_OBJECT
public:
    explicit DistanceSensor(int trigPin,QString name, QObject *parent = nullptr);



};

#endif // DISTANCESENSOR_H
