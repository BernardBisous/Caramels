#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#include "hardware/device.h"
#include "hardware/distancepoller.h"
#include <QObject>

class DistanceSensor : public Sensor
{
    Q_OBJECT
public:
    explicit DistanceSensor(int echoPin, int trigPin,QString name, QObject *parent = nullptr);
    virtual void begin();
    virtual void reactToDataEdited(QString key,QString val);
    float gain();

public slots:
    virtual void measure();

signals:
    void aquiered(float t);

private slots:
    void addMeasurment(float dist);

private:
    DistancePoller* m_poller;
    int m_echoPin;
    int m_trigPin;
    int m_maxSize;


};

#endif // DISTANCESENSOR_H
