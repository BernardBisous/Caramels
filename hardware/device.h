#ifndef DEVICE_H
#define DEVICE_H

#include "hardware/serialtent.h"
#include "parameter.h"
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
    virtual float neutralPurcent(){return 0;}
    virtual QString diagnose(bool*){return "";}

    virtual void retreiveLastValue();
    virtual float computeResult(QString){return 0;}

    virtual QString userValue();

    void computeResults();

    int possibleParameterId();
    bool acceptsParam(int id);

    QString dataValue(QString key) const;
    bool existData(QString key);
    void setDataValue(QString key, QString val, bool fromEditing=false);

    void loadSettings();
    void setResult(QString key,QString units="");
    QStringList resultKeys();
    QStringList dataKeys();
    QString name() const;
    void setName(const QString &newName);

    QString storageFile();
    void startRecording(bool t);





    void setRange(float min, float max);
    void setMaxRange(float mx);

    float mapToPurcent(float val);
    float maxRange();
    float minRange();
    void computeGains();

    static bool createDataDir();

    float currentPurcent();
    virtual float currentValue() const;

    SerialTent *serial() const;
    void setSerial(SerialTent *newSerial);

    QString units() const;
    void setUnits(const QString &newUnits);

    QList<RealTimeValue> values() const;

    float gain() const;
    void exportHistoric(QString file);
    void clearHistoric();
    QList<RealTimeValue> historic();
    QList<RealTimeValue> historic(int size);

    void setRecordDelay(int newRecordDelay);

    bool continousStreaming() const;

    void attachParameter(Parameter*p);

    Parameter *parameter() const;

    QHash<QString, float> metaResults() const;

    QHash<QString, QString> resultsUnits() const;
    void setResultUnits(QString key,QString units);




    bool enabled() const;
    void enable(bool newEnable);

    int maxValues() const;

public slots:
    void storeValue(RealTimeValue t);


signals:
    void log(QString s);
    void newValue(float t);
    void error(QString diagnose, bool warning);


protected :

    void appendValue(float v);
    QList<RealTimeValue> m_values;
    int m_maxValues;

    QHash<QString,QString> m_metaData;
    QHash<QString,float>m_metaResults;
    QHash<QString,QString> m_resultsUnits;


    bool m_recording;
    int m_deviceId;

    QString m_name;
    QString m_units;

    SerialTent* m_serial;

    float m_gain;
    float m_offset;

    QDateTime m_lastRecord;
    int m_recordDelay;

    bool m_continousStreaming;

    Parameter* m_parameter;

    bool m_enable;
};



class Sensor : public Device
{
    Q_OBJECT
public:
    explicit Sensor(int pin, QString name="Sensor", QObject *parent = nullptr);
    virtual float aquire();
    virtual void begin();

public slots:
    virtual void measure();

protected :
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
     virtual void retreiveLastValue();
    virtual QString userValue();
    void switchStateUser();
    void setStateHigh(bool s=true);

    void applyValue(float);
    void userApplyPurcent(float);
    virtual void reset();
    void stop();
    void impulse(float val, int ms, float valEnd);
    void impulseHigh(int ms);
    void setIntegralUnits(QString s);

    virtual float computeResult(QString);
    void computeHistoricIntegral();
    QList<RealTimeValue> integralHistoric();

    float integral() const;
    float immediateIntegral() const;
    QString integralUnit();

    bool integratedInteresting() const;
    void setIntegratedInteresting(bool newIntegratedInteresting);

signals:
    void impulseDone();

private slots:
    void impulseSlot();

protected :

    bool m_integratedInteresting;
    float m_integral;
    float m_nextVal;
    QString m_lastDiagnosis;
    QTimer* m_impulseTimer;
};

class SwitchedActuator : public Actuator
{
    Q_OBJECT
public:
    explicit SwitchedActuator (int m_pin,bool pwm, QString name="Switched Actuator",QObject *parent = nullptr);
    virtual float filterInputValue(float v);
    virtual void applyPurcent(float v);

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
    explicit Motor (int pin1,int pin2, QString name="Motor",QObject *parent = nullptr);
    virtual void applyValue(float v);
    virtual void applyPurcent(float v);
    virtual float neutralPurcent(){return 50;}
    void setMaxSpeed(float v);
    float maxSpeed();
    void move(float t);

public slots:


protected :

private:

    bool m_clockwise;
    int m_pin1;
    int m_pin2;
};




#endif // DEVICE_H
