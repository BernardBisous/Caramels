#ifndef CONFIGOVERVIEW_H
#define CONFIGOVERVIEW_H

#include "configprogress.h"
#include "configtop.h"
#include "eventmanager.h"
#include "qlabel.h"

#include "statewidget.h"
#include "tent.h"
#include <QWidget>
#include "webcamwidget.h"

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
    ConfigTop* m_topStart;

    StateWidget* m_state;
};

#endif // CONFIGOVERVIEW_H
