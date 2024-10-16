#ifndef DEVICETESTER_H
#define DEVICETESTER_H

#include "hardware/device.h"
#include "hardware/tent.h"
#include <QObject>

class DeviceTester : public QObject
{
    Q_OBJECT
public:
    explicit DeviceTester(Device* d,Tent* t,QObject *parent = nullptr);
    void run();
    void setIntervalActuator(int i);
    float consupmtion();
    QString subText();

    AnalogSensor *sensor() const;

signals:
    void newCurrent(float t);

private slots:
    void timerSlot();

private:
    Device* m_device;
    Actuator* m_actuator;
    Tent* m_tent;
    AnalogSensor* m_sensor;
    QTimer* m_switchTimer;
    QList<float> m_high;
    QList<float>  m_low;

};

#endif // DEVICETESTER_H
