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
    virtual void update();
    virtual void reactToParamChanged(Parameter*, float ){}

    void attachDevice(Device* d);
    virtual void attachParameter(Parameter* p);
    Device* devFromName(QString s);
    Parameter* paramFromName(QString s);
    QStringList parametersName();
    Parameter* parameterFromId(int id);

    QList<Device *> devices() const;
    QList<Parameter *> parameters() const;

    QString name() const;

    QList<int> idParameters() const;


    QDateTime startTime() const;
    void setStartTime(const QDateTime &newStartTime);


signals:

    void finished();
private slots:
    void timerSlot();

protected:
    QList<int> m_idParameters;
    QString m_name;
    QList<Device*> m_devices;
    QList<Parameter*> m_parameters;

    QDateTime m_startTime;

private:
    QTimer* m_timer;

};

#endif // HARDWAREUNIT_H
