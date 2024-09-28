#ifndef DEVICESTATE_H
#define DEVICESTATE_H

#include <QObject>
#include "hardware/hardwareunit.h"


class DeviceState:  public QObject
{
    Q_OBJECT
public:
    typedef enum criticityState{Good,Warning,Danger}Criticity;
    explicit DeviceState(Device*d,HardwareUnit*h,QObject*parent);
    virtual void refresh();
    virtual QString diagnosis() const;
    virtual QString name();
    void setState(QString d,Criticity s);
    QString fullDiagnosis();

    HardwareUnit *unit() const;

    Device *device() const;

    Criticity criticity() const;



signals:
    void changed();

public slots:
    void clientError(QString s, bool warning);

private:
    HardwareUnit* m_unit;
    Device* m_device;

    Criticity m_criticity;
    QString m_diagnosis;
};

class SerialState:  public DeviceState
{
    Q_OBJECT
public:
    explicit SerialState(SerialTent* c,QObject*parent);
    virtual QString diagnosis() const;
    virtual QString name();
    virtual void refresh();

private slots:
    void changedState(bool s);
private:
    SerialTent* m_client;

};

#endif // DEVICESTATE_H
