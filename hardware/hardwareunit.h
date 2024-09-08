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
    virtual void begin();
    virtual void update(int index);
    virtual void reactToParamChanged(Parameter*, float ){}
    virtual void reactToSensorsChanged(){}
    virtual void finish();

    bool activeConfig();

    void attachDevice(Device* d);
    virtual void attachParameter(Parameter* p);
    Device* devFromName(QString s);
    Parameter* paramFromName(QString s);
    QStringList parametersName();
    Parameter* parameterFromId(int id);
    void exportAll(QString dir);

    bool canHandleParameter(int id);

    void updateSensors();

    QList<Device *> devices() const;
    QList<Parameter *> parameters() const;


    QString name() const;

    QList<int> idParameters() const;
    QList<Sensor*> sensors();


    QDateTime startTime() const;
    void setStartTime(const QDateTime &newStartTime);


    void console(QString s){emit consoleRequest(s);}

signals:

    void finished();
    void consoleRequest(QString);


protected:
    QList<int> m_idParameters;
    QString m_name;
    QList<Device*> m_devices;
    QList<Sensor*> m_sensors;

    QList<Parameter*> m_parameters;
    QDateTime m_startTime;
};

#endif // HARDWAREUNIT_H
