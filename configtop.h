#ifndef CONFIGTOP_H
#define CONFIGTOP_H

#include "Interface/progresswidget.h"
#include <QWidget>

class ConfigTop : public ProgressWidget
{
    Q_OBJECT
public:
    explicit ConfigTop(QWidget *parent = nullptr);


public slots:
    void relayStart(){emit started();stop();}
signals:
    void started();

};

#endif // CONFIGTOP_H
