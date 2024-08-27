#ifndef OVERVIEW_H
#define OVERVIEW_H

#include "Interface/progresswidget.h"
#include "configoverview.h"
#include "hardwareoverview.h"
#include "tent.h"
#include <QWidget>

class Overview : public QWidget
{
    Q_OBJECT
public:
    explicit Overview(QWidget *parent = nullptr);
    void loadHardware(Tent* t);
    TopWidget* emptyWidget();

public slots:
    void createSlot();

signals:

private:
    Tent* m_tent;
    TopWidget* m_emptyWidget;
    HardwareOverview* m_hardware;
    ConfigOverview* m_config;

};

#endif // OVERVIEW_H
