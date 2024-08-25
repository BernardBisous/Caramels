#ifndef PUMP_H
#define PUMP_H

#include "hardware/device.h"
#include <QObject>

class Pump : public SwitchedActuator
{
    Q_OBJECT
public:
    explicit Pump(int pin,QString name="Pump", QObject *parent = nullptr);

signals:

};

class ChemicalPump : public SwitchedActuator
{
    Q_OBJECT
public:
    explicit ChemicalPump(int pin,QString name="Injector", QObject *parent = nullptr);
    void inject(float volumeML);

signals:

};

#endif // PUMP_H
