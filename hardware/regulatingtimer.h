#ifndef REGULATINGTIMER_H
#define REGULATINGTIMER_H

#include "hardware/device.h"
#include "qdatetime.h"
#include "qtimer.h"
#include <QObject>

class RegulatingTimer : public QObject
{
    Q_OBJECT
public:
    explicit RegulatingTimer(QObject *parent = nullptr);
    void setDelaysHours(float delayHigh, float delayLow);

    void reset();
    void startState(bool high);
    bool shouldChange();
    void setDelayHigh(float h);
    void setDelayLow(float h);
    QDateTime nextSwitch();
    float remainingHours();

    QString highString() const;
    void setHighString(const QString &newHighString);

    QString lowString() const;
    void setLowString(const QString &newLowString);
    void setStateStrings(QString h,QString low);

    QString currentString();

    void userSwitch();

    bool state() const;

    Actuator *device() const;
    void setDevice(Actuator *newDevice);

    float high() const;

    float low() const;
    void continueTimer();

private slots:
    void timerSlot();

signals:
    void shouldSwitch(bool s);


private:
    QTimer* m_timer;
    QDateTime m_lastSwitch;

    bool m_state;
    float m_high;
    float m_low;

    QString m_highString;
    QString m_lowString;

    Actuator* m_device;
    float m_hoursRoutine;

    bool m_startsHigh;

};

#endif // REGULATINGTIMER_H
