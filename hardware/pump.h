#ifndef PUMP_H
#define PUMP_H

#include "hardware/device.h"
#include "qelapsedtimer.h"
#include <QObject>

class Pump : public SwitchedActuator
{
    Q_OBJECT
public:
    explicit Pump(int pin,QString name="Pump", QObject *parent = nullptr);
    void inject(float volumeML);
    void startInjecting(bool s);
    void setFlow(float mLPerSec);

signals:


private:


};


#endif // PUMP_H
