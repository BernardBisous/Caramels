#ifndef EVENTS_H
#define EVENTS_H


#include "qlist.h"

class Event
{
public:
    QString name;
    int hourIndex;
};

class Events
{
public:
    Events();
    void clear();
    void save(QDataStream&c);
    bool load(QDataStream&c);
    void add(QString name, int h);
    void reset();
    Event next();
    Event at(int i);
    int count();
    QList<Event> list() const;
    void skipNext();
    QString storageFile(){return "events.txt";}
    void registerLastEvent(int secnds);

private:
    QList<Event> m_list;
    QList<Event> m_Totallist;
};

#endif // EVENTS_H
