#ifndef TENT_H
#define TENT_H

#include "growconfig.h"
#include "qdatetime.h"
#include <QObject>

class Tent : public QObject
{
    Q_OBJECT
public:
    explicit Tent(QObject *parent = nullptr);
    QString configName() const;
    void setConfig(GrowConfig*e);

    void restart();
    GrowConfig *config() const;

signals:

private:
    GrowConfig* m_config;
    QString m_configName;
    QDateTime m_startedDate;
};

#endif // TENT_H
