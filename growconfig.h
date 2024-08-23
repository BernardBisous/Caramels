#ifndef GROWCONFIG_H
#define GROWCONFIG_H

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
