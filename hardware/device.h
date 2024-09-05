#ifndef DEVICE_H
#define DEVICE_H

#include "hardware/serialtent.h"
#include "qdatetime.h"
#include "qtimer.h"
#include <QObject>

#include <QHash>



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
    virtual void reset();

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

    void retreiveLastValue();



    void setRange(float min, float max);
    void setGain(float t);
    void setOffset(float t);


    float maxRange();
    float minRange();

    static bool createDataDir();

    float currentPurcent();
    float currentValue() const;

    SerialTent *serial() const;
    void setSerial(SerialTent *newSerial);

    QString units() const;
    void setUnits(const QString &newUnits);

    QList<RealTimeValue> values() const;

    float gain() const;
    void exportHistoric(QString file);
    void clearHistoric();

    void setRecordDelay(int newRecordDelay);

public slots:
    void storeValue(float t);


signals:
    void log(QString s);
    void newValue(float t);

protected :
    QList<RealTimeValue> historic();
    QList<RealTimeValue> historic(int size);
    void appendValue(float v);
    QList<RealTimeValue> m_values;

    QHash<QString,QString> m_metaData;
    QHash<QString,float> m_metaResults;
    bool m_recording;
    int m_deviceId;

    QString m_name;
    QString m_units;

    SerialTent* m_serial;

    float m_gain;
    float m_offset;

    QDateTime m_lastRecord;
    int m_recordDelay;
};



class Sensor : public Device
{
    Q_OBJECT
public:
    explicit Sensor(int pin, QString name="Sensor", QObject *parent = nullptr);
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


    int m_pin;


};



class Actuator : public Device
{
    Q_OBJECT
public:
    explicit Actuator(QString name="Acturator",QObject *parent = nullptr);
    void test();
    virtual float filterInputValue(float v){return v;}
    virtual void applyPurcent(float v);
    void applyValue(float);
    void userApplyPurcent(float);
    virtual void reset();
    void stop();
    void impulse(float val, int ms, float valEnd);
    void impulseHigh(int ms);

signals:
    void impulseDone();

private slots:
    void impulseSlot();

protected :

    float m_nextVal;
    QTimer* m_impulseTimer;
};

class SwitchedActuator : public Actuator
{
    Q_OBJECT
public:
    explicit SwitchedActuator (int m_pin,bool pwm, QString name="Switched Actuator",QObject *parent = nullptr);
    virtual void applyPurcent(float v);
    virtual float filterInputValue(float v);

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
    virtual void applyValue(float v);

public slots:


protected :

private:
    int m_dirPin;
    int m_pwmPin;
};


#endif // DEVICE_H
