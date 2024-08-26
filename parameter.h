#ifndef PARAMETER_H
#define PARAMETER_H


#include "Interface/scrollarea.h"
#include "hardware/device.h"

typedef enum parameterType{LightPower,LightSpectre}ParameterType;

class TimedValue
{
public:
    int hourIndex;
    float value;
};

class Parameter
{
public:

    Parameter(QString name="NoName",QString units="", int id=0);

    static float timeMultiplicator();
    QList<TimedValue> values() const;
    void load(QDataStream&c);
    void save(QDataStream&c);
    int count();
    int maxX();
    float maxY();
    QPointF at(int i);

    float currentValue(QDateTime startTime,bool*e);
    float valueAtTime(int h,bool*e);

    void clear();

    int append(TimedValue t);

    void setName(const QString &newName);

    void setUnits(const QString &newUnits);

    void setId(int newId);

    QString name() const;

    int id() const;

    QString units() const;

    QString description() const;
    void setDescription(const QString &newDescription);

    QString group() const;
    void setGroup(const QString &newGroup);


private:
    int m_id;
    ParameterType m_type;
    QString m_name;
    QString m_units;
    QString m_description;
    QString m_group;


    QList<TimedValue> m_values;

};


#endif // PARAMETER_H
