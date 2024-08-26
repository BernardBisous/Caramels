#ifndef DEVICE_H
#define DEVICE_H

#include "qdatetime.h"
#include "qtimer.h"
#include <QObject>

#include <QHash>

#include "rasppi.h"

class RealTimeValue
{
public:
    QDateTime time;

    float value;
};

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QString namme="Device",QObject *parent = nullptr);
    virtual void begin();
    virtual void save(QDataStream& s);
    virtual void load(QDataStream& s);
    virtual void reactToDataEdited(QString key,QString val);

    virtual float computeResult(QString){return 0;}
    void computeResults();


    int possibleParameterId();
    bool acceptsParam(int id);

    QString dataValue(QString key);
    bool existData(QString key);
    void setDataValue(QString key, QString val, bool fromEditing=false);



    void setResult(QString key);
    QStringList resultKeys();
    QStringList dataKeys();
    QString name() const;
    void setName(const QString &newName);

    QString storageFile();
    void startRecording(bool t);
    QList<RealTimeValue> historic();
    void retreiveLastValue();





    static bool createDataDir();

    float currentValue() const;

public slots:
    void storeValue(float t);


signals:
    void log(QString s);
    void newValue(float t);

protected :

    void appendValue(float v);

    QHash<QString,QString> m_metaData;
    QHash<QString,float> m_metaResults;
    bool m_recording;
    int m_deviceId;

    QString m_name;
    float m_currentValue;

};



class Sensor : public Device
{
    Q_OBJECT
public:
    explicit Sensor(QString name="Sensor",QObject *parent = nullptr);
    virtual float aquire();
    virtual void reactToDataEdited(QString key,QString val);
    virtual void begin();
    void updateSamplingRate();
    void startPolling(bool s);

public slots:
    virtual void measure();

protected :

    QTimer* m_pollTimer;
    bool m_continousStreaming;

};



class Actuator : public Device
{
    Q_OBJECT
public:
    explicit Actuator(QString name="Acturator",QObject *parent = nullptr);
    void test();
    virtual void applyValue(float v, int ms=-1);

    void applyPurcent(int o);
    virtual float filterInputValue(float v){return v;}


protected :
};

class SwitchedActuator : public Actuator
{
    Q_OBJECT
public:
    explicit SwitchedActuator (int m_pin,bool pwm, QString name="Switched Actuator",QObject *parent = nullptr);
    virtual float filterInputValue(float v);
    virtual void begin();
    virtual void applyValue(float v,int ms=-1);

public slots:


protected :

private:

    int m_pin;
    bool m_pwmAnalog;
};

class Motor : public Actuator
{
    Q_OBJECT
public:
    explicit Motor (int dirpin,int pwm, QString name="Motor",QObject *parent = nullptr);
    virtual void begin();
    virtual void applyValue(float v,int ms);

public slots:


protected :

private:
    int m_dirPin;
    int m_pwmPin;
};


#endif // DEVICE_H
