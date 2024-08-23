#ifndef PUMP_H
#define PUMP_H

#include "hardware/device.h"
#include <QObject>

class Pump : public SwitchedActuator
{
    Q_OBJECT
public:
    explicit Pump(int pin, QObject *parent = nullptr);

signals:

};

#endif // PUMP_H
