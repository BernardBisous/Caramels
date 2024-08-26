#ifndef DISTANCEPOLLER_H
#define DISTANCEPOLLER_H

#include <QObject>


class DistancePoller : public QObject
{
    Q_OBJECT

public:
    explicit DistancePoller(QObject *parent = nullptr);
    float calculateDistance();
    void startPolling(int echi, int trig, int n);

signals:
     void distanceMeasured(float distance);

private:
    QThread *m_thread;
    int m_Echopin;
    int m_trigPin;

    // Function to poll the sensor and emit the distance
    void pollSensor(int n);
};

#endif // DISTANCEPOLLER_H
