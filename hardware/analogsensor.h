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
    int regulationDelay();
    void setRegulationDelay(int seconds);
    bool shouldRegulate();
    virtual void begin();

    QDateTime nextRegulation();
    float smoothedValue()const;
    virtual void reset();

    void regulateNow();
    void setRegulated();

    float command() const;
    void setCommand(float newCommand);
    float errorValue();
    virtual QString userValue();



signals:
    void regulated();

public slots:
    virtual void measure();

private:
    QDateTime m_lastRegTime;
    QList<float> m_smoothedValues;
    float m_command;
};

#endif // ANALOGSENSOR_H
