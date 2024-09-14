#ifndef BOOLEANSENSOR_H
#define BOOLEANSENSOR_H

#include "hardware/device.h"
#include <QObject>

class BooleanSensor : public Sensor
{
    Q_OBJECT
public:
    explicit BooleanSensor( int pin, QString n, QObject *parent = nullptr);
    virtual QString userValue();

    bool activeHigh() const;
    void setActiveHigh(bool newActiveHigh);

private:
    bool m_activeHigh;

};

#endif // BOOLEANSENSOR_H
