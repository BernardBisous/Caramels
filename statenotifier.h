#ifndef STATENOTIFIER_H
#define STATENOTIFIER_H




#include "devicestate.h"
#include <QObject>


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
    void append(DeviceState*s);
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
   // void emailErrors();

signals:
    void changed();
    void stateChanged();

private:
    QList<DeviceState*> m_list;
    QList<Device*> m_hiddenStates;
    DeviceState::Criticity m_criticity;
    //QTimer* m_emailTimer;
};

#endif // STATENOTIFIER_H
