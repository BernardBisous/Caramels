#ifndef EVENTWIZZARD_H
#define EVENTWIZZARD_H

#include "config/events.h"
#include "widgets/wizzard.h"
#include <QWidget>


class EventWizzard : public Wizzard
{
    Q_OBJECT
public:
    explicit EventWizzard(Event* e,QWidget*parent=nullptr);
    static EventWizzard *executeEvent(Event* e);
    void loadEventName(QString name);
signals:
private:
    Event* m_client;

};


class FinalWizzard : public EventWizzard
{
    Q_OBJECT
public:
    explicit FinalWizzard(Event* e,QWidget*parent=nullptr);


};


#endif // EVENTWIZZARD_H
