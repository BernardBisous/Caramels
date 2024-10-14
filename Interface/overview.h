#ifndef OVERVIEW_H
#define OVERVIEW_H


#include "Interface/progresswidget.h"
#include "widgets/configoverview.h"
#include "widgets/consolewidget.h"
#include "widgets/hardwareoverview.h"
#include "hardware/tent.h"
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



};

#endif // OVERVIEW_H
