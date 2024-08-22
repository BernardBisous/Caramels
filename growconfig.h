#ifndef GROWCONFIG_H
#define GROWCONFIG_H
#include "qdatetime.h"
#include <QList>
#include <QString>

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

    QList<TimedValue> values() const;
    void load(QDataStream&c);
    void save(QDataStream&c);
    int count();
    int maxX();
    float maxY();

    void clear();

    int append(TimedValue t);

    void setName(const QString &newName);

    void setUnits(const QString &newUnits);

    void setId(int newId);

    QString name() const;

    int id() const;

    QString units() const;



private:
    int m_id;
    ParameterType m_type;
    QString m_name;
    QString m_units;
    QList<TimedValue> m_values;
};

class GrowConfig
{
public:
    GrowConfig();

    bool save();
    bool save(QString s);
    int countParameters();
    bool loadCsv(QString filename);

    void save(QDataStream&c);
    bool load(QDataStream&c);

    int maxHours();
    float maxY();

    bool openDefault();
    bool browse();

    bool open(QString filename);

    static GrowConfig open(bool*ok);

    int getSize() const;
    void setSize(int newSize);

    QStringList parameterNames();
    Parameter* parameterAddr(int i);

    Parameter* createDefault();

    void defaultSetup();
    enum CSVformat{Group=0,Name,Units,Id,Description,Values};
    Parameter* addParameter(QString s="noName",QString units="", int id=0);
    Parameter* fromName(QString );


private:
    int size;//total size in hours

    QList<Parameter*> m_parameters;
};

#endif // GROWCONFIG_H
