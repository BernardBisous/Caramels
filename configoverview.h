#ifndef CONFIGOVERVIEW_H
#define CONFIGOVERVIEW_H

#include "Interface/toolbutton.h"
#include "configprogress.h"
#include "configtop.h"
#include "eventmanager.h"
#include "qlabel.h"

#include "tent.h"
#include <QWidget>
#include "webcamoverview.h"

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
    ToolButton* m_restartButton;
    ConfigProgress* m_progress;

    CameraOverview* m_cam;

    EventManager* m_event;
    ConfigTop* m_topStart;
};

#endif // CONFIGOVERVIEW_H
