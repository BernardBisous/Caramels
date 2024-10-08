#ifndef PARAMETER_H
#define PARAMETER_H

#include <QString>
#include <QList>
#include<QDateTime>

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
    TimedValue at(int i);
    float currentValue(QDateTime startTime,bool*e);
    float valueAtTime(int h,bool*e);
    int closerIndex(int h);
    void clear();
    int append(TimedValue t);

    void load(QDataStream&c);
    void save(QDataStream&c);
    int count();
    int maxX();
    QString csvLine(QString sep);
    float maxY();
    float minY();
    void rangeY(float*max,float*min);

    void movePoints(QList<int>indexes,float value, bool offset);
    void removePoint(QList<int>indexes);
    void addPoint(int indexHour, float value);
    void addPoint(TimedValue t);
    void addDefaultPoint(int index);


    QString userValueAt(int i);
    void setName(const QString &newName);
    QString name() const;

    void setUnits(const QString &newUnits);
    QString units() const;

    int id() const;
    void setId(int newId);

    QString description() const;
    void setDescription(const QString &newDescription);


private:
    int m_id;
    QString m_name;
    QString m_units;
    QString m_description;
    QList<TimedValue> m_values;
};



#endif // PARAMETER_H
