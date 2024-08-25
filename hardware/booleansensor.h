#ifndef BOOLEANSENSOR_H
#define BOOLEANSENSOR_H

#include "hardware/device.h"
#include <QObject>

class BooleanSensor : public Sensor
{
    Q_OBJECT
public:
    explicit BooleanSensor( int pin, QString n, QObject *parent = nullptr);

signals:

private:
    int m_pin;

};

#endif // BOOLEANSENSOR_H
