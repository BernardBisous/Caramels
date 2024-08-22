#ifndef DEVICE_H
#define DEVICE_H

#include "growconfig.h"
#include <QObject>




class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    virtual void begin();
    virtual void save(QDataStream& s);
    virtual void load(QDataStream& s);
    int possibleParameterId();
    void attach(Parameter* p);
    Parameter *client() const;

    QString dataValue(QString key);
    bool existData(QString key);
    void setDataValue(QString key, QString val);


signals:
    void log(QString s);

protected :
    QHash<QString,QString> m_metaData;
    int m_deviceId;
    int m_paramId;
    QString m_name;
    Parameter*m_client;
};

class Sensor : public Device
{
    Q_OBJECT
public:
    explicit Sensor(QObject *parent = nullptr);

protected :

};

class Actuator : public Device
{
    Q_OBJECT
public:
    explicit Actuator(QObject *parent = nullptr);
    void test();


    void applyValue(float v);

protected :
};

class SwitchedActuator : public Actuator
{
    Q_OBJECT
public:
    explicit SwitchedActuator (int m_pin,bool pwm, QObject *parent = nullptr);

protected :

private:
    int m_pin;
    bool m_pwmAnalog;
};
#endif // DEVICE_H
