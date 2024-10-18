#ifndef EVENTPENDINGPROGRESS_H
#define EVENTPENDINGPROGRESS_H

#include "Interface/progresswidget.h"
#include "config/events.h"
#include <QWidget>

class EventPendingProgress : public ProgressWidget
{
    Q_OBJECT
public:
    explicit EventPendingProgress(QWidget *parent = nullptr);
    void setEvent(Event e);

private slots:
    void skipSlot();
    void startSlot();

signals:
    void skip();
    void startEvent();

};

#endif // EVENTPENDINGPROGRESS_H
