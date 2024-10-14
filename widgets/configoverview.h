#ifndef CONFIGOVERVIEW_H
#define CONFIGOVERVIEW_H

#include <QWidget>
#include "qlabel.h"

#include "widgets/configprogress.h"
#include "widgets/configtop.h"

#include "widgets/webcamwidget.h"

#include "widgets/statewidget.h"
#include "hardware/tent.h"
#include "config/eventmanager.h"


class ConfigOverview : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigOverview(QWidget *parent = nullptr);
    void setTent(Tent*t);
    void refresh();

signals:
    void edit(HardwareUnit* t);

private slots:
    void restart();
    void updateDate();
    void valueSlot(int i);
    void actionTop(QString,ActionWidget*);

private:
    QLabel* m_name;
    QLabel* m_start;
    Tent* m_client;
  //  ToolButton* m_restartButton;
    ConfigProgress* m_progress;

    WebcamWidget* m_cam;

    EventManager* m_event;
    ProgressWidget* m_topStart;

    StateWidget* m_state;
};

#endif // CONFIGOVERVIEW_H
