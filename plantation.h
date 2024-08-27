#ifndef PLANTATION_H
#define PLANTATION_H

#include "growconfig.h"
#include "qdatetime.h"
#include <QObject>

class Plantation : public QObject
{
    Q_OBJECT
public:
    explicit Plantation(QString variety,QString configName,QObject *parent = nullptr);



signals:

private:
    QDateTime m_startDate;
    QString m_variety;
    GrowConfig* m_config;

};

#endif // PLANTATION_H
