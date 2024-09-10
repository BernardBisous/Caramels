#ifndef ANALOGSENSOR_H
#define ANALOGSENSOR_H

#include "hardware/device.h"
#include <QObject>

class AnalogSensor : public Sensor
{
    Q_OBJECT
public:
    explicit AnalogSensor(int pin,QString n,QObject *parent = nullptr);
    void setSmoothing(int values);
    int smoothing()const;
    void setRegulationDelay(int seconds);
    bool shouldRegulate();
    float smoothedValue()const;
    virtual void reset();

public slots:
    virtual void measure();

private:
    QDateTime m_lastRegTime;
    QList<float> m_smoothedValues;
};

#endif // ANALOGSENSOR_H
