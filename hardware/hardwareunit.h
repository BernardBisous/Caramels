#ifndef HARDWAREUNIT_H
#define HARDWAREUNIT_H

#include "hardware/analogsensor.h"
#include "hardware/device.h"
#include "hardware/regulatingtimer.h"
#include "config/parameter.h"
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
    virtual void attachParameter(Parameter* p);
    virtual QList<Actuator*> interestingIntegrals(){return actuators(true);}
    virtual QList<Device*> interestingDevices(){return coupledDevices();}
    virtual QString diagnose(bool*){return "";}

    virtual QStringList trigKeys(){return QStringList();}
    virtual void trigKey(QString s);

    virtual AnalogSensor*regulatingSensor();


    void attachCouples(int paramId,Device*e);
    Device* attachedDevice(Parameter*p);

    bool activeConfig();

    void attachDevice(Device* d);

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
    QList<Actuator*> actuators(bool integratedOnly=false);

    QDateTime startTime() const;
    void setStartTime(const QDateTime &newStartTime);




    void console(QString s){emit consoleRequest(s);}

    QDateTime endConfig();
    void computeEndTime();

    QList<Device*> coupledDevices();





    QString description() const;
    void setDescription(const QString &newDescription);

    RegulatingTimer *regulator() const;
    void setRegulator(RegulatingTimer *newRegulator);
    void setTimeRegulated(Actuator* a);



signals:

    void finished();
    void consoleRequest(QString);
    void startChanged(QDateTime start);
    void newParameters();
    void error(QString s,bool w);



protected:

    QHash<int,Device*> m_couples;

    QList<int> m_idParameters;
    QString m_name;
    QList<Device*> m_devices;
    QList<Sensor*> m_sensors;

    QList<Parameter*> m_parameters;
    QDateTime m_startTime;
    QDateTime m_endTime;

    QString m_description;

    RegulatingTimer* m_regulator;

};




#endif // HARDWAREUNIT_H
