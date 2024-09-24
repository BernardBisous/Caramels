#ifndef STATENOTIFIER_H
#define STATENOTIFIER_H



#include "hardware/hardwareunit.h"
#include <QObject>

class DeviceState:  public QObject
{
    Q_OBJECT
public:
    typedef enum criticityState{Good,Warning,Danger}Criticity;
    explicit DeviceState(Device*d,HardwareUnit*h,QObject*parent);
    void refresh();
    void setState(QString d,Criticity s);
    QString fullDiagnosis();

    HardwareUnit *unit() const;

    Device *device() const;

    Criticity criticity() const;

    QString diagnosis() const;

signals:
    void changed();

private slots:
    void clientError(QString s, bool w);

private:
    HardwareUnit* m_unit;
    Device* m_device;

    Criticity m_criticity;
    QString m_diagnosis;
};


class StateNotifier : public QObject
{
    Q_OBJECT
public:
    explicit StateNotifier(QObject *parent = nullptr);
    void hide(Device*d);
    void refresh();
    void reset();
    void append(HardwareUnit*u);
    void append(QList<Device*> dl);
    void append(Device* d);
    bool isOk();
    DeviceState* worst();
    void removeDevice(Device*d);
    void removeUnit();

    void updateCriticity();
    DeviceState* unitState(HardwareUnit*u);
    DeviceState*state(Device*s);

    QList<DeviceState*> bads();

    DeviceState::Criticity criticity() const;

private slots:
    void errorSlot();
    void emailErrors();

signals:
    void changed();
    void stateChanged();

private:
    QList<DeviceState*> m_list;
    QList<Device*> m_hiddenStates;
    DeviceState::Criticity m_criticity;
    QTimer* m_emailTimer;
};

#endif // STATENOTIFIER_H
