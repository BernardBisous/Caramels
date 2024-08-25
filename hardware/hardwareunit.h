#ifndef HARDWAREUNIT_H
#define HARDWAREUNIT_H

#include "hardware/device.h"
#include "parameter.h"
#include <QObject>

class HardwareUnit : public QObject
{
    Q_OBJECT
public:
    explicit HardwareUnit(QObject *parent = nullptr);
    void attachDevice(Device* d);
    void attachParameter(Parameter* p);
    int parameterPossibleId();
    Device* devFromName(QString s);
    void renameDev(Device*d);

    QList<Device *> devices() const;
    QList<Parameter *> parameters() const;

signals:

protected:
    int m_idParameter;
    QString m_name;
    QList<Device*> m_devices;
    QList<Parameter*> m_parameters;

};

#endif // HARDWAREUNIT_H
