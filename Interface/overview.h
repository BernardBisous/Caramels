#ifndef OVERVIEW_H
#define OVERVIEW_H

#include "Interface/plantationeditor.h"
#include "Interface/progresswidget.h"
#include "configoverview.h"
#include "consolewidget.h"
#include "hardwareoverview.h"
#include "tent.h"
#include <QWidget>

class Overview : public QWidget
{
    Q_OBJECT
public:
    explicit Overview(QWidget *parent = nullptr);
    void loadHardware(Tent* t);

public slots:
    void createSlot();
    void editSlot(HardwareUnit* t){emit editOne(t);}


signals:
    void editOne(HardwareUnit* t);
private:
    Tent* m_tent;
    TopWidget* m_emptyWidget;
    HardwareOverview* m_hardware;
    ConfigOverview* m_config;
    PlantationEditor* m_plantation;
    ConsoleWidget* m_console;


};

#endif // OVERVIEW_H
