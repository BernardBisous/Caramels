#ifndef PUMP_H
#define PUMP_H

#include "hardware/device.h"
#include <QObject>

class Pump : public SwitchedActuator
{
    Q_OBJECT
public:
    explicit Pump(int pin,QString name="Pump", QObject *parent = nullptr);
    void inject(float volumeML);
    float flow(); //mL per secons

signals:

};


#endif // PUMP_H
