#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#include "hardware/device.h"

#include <QObject>

class DistanceSensor : public Sensor
{
    Q_OBJECT
public:
    explicit DistanceSensor(int trigPin,QString name, QObject *parent = nullptr);



};

#endif // DISTANCESENSOR_H
