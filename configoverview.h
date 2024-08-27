#ifndef CONFIGOVERVIEW_H
#define CONFIGOVERVIEW_H

#include "growconfig.h"
#include <QWidget>

class ConfigOverview : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigOverview(QWidget *parent = nullptr);
    void handle(GrowConfig*c);
    void setStartDate(QDateTime);

signals:

};

#endif // CONFIGOVERVIEW_H
