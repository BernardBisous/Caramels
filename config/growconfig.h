#ifndef GROWCONFIG_H
#define GROWCONFIG_H

#include "config/events.h"
#include "qdatetime.h"
#include <QList>
#include <QString>
#include "parameter.h"



class GrowConfig
{
public:
    GrowConfig();

    bool save();
    bool save(QString s);
    int countParameters();
    void loadArchive(QString dir);
    bool loadCsv(QString filename);
    int indexOf(Parameter*c);
    void save(QDataStream&c);
    bool load(QDataStream&c);
    int computeMaxHours();
    float maxY();
    void clear();

    Parameter* loadParameterCSVLine(QString s,QStringList header);
    bool openDefault();
    bool browse();

    bool open(QString filename);

    static GrowConfig open(bool*ok);

    QStringList parameterNames();
    Parameter* parameterAddr(int i);

    Parameter* createDefault();

    void defaultSetup();
    enum CSVformat{Group=0,Name,Units,Id,Description,Values};
    Parameter* addParameter(QString s="noName",QString units="", int id=0);
    Parameter* fromName(QString );

    void start();

    QString name() const;
    void setName(const QString &newName);

    int maxHours() const;

    Events *events() const;

    void saveCsv(QString file);

    float getResult() const;
    void setResult(float newResult);

private:
    QList<Parameter*> m_parameters;
    Events* m_events;
    QString m_name;
    int m_maXHours;
    float m_result;
};



#endif // GROWCONFIG_H
