#ifndef ANALOGSENSOR_H
#define ANALOGSENSOR_H

#include "hardware/device.h"
#include <QObject>

class AnalogSensor : public Sensor
{
    Q_OBJECT
public:
    explicit AnalogSensor(int pin,QString n,QObject *parent = nullptr);
};

#endif // ANALOGSENSOR_H
