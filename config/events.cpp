#include "events.h"
#include "qdatetime.h"
#include "qdebug.h"

#include <QFile>
Events::Events()
{

}

void Events::clear()
{
    m_list.clear();
    m_Totallist.clear();
}

void Events::add(QString name, int h)
{
    Event e;
    e.name=name;
    e.hourIndex=h;
    m_list.append(e);
    m_Totallist.append(e);
}

void Events::reset()
{
    m_list=m_Totallist;
}

Event Events::next()
{
    if(m_list.isEmpty())
        return Event{"",0};

    return m_list.first();
}

Event *Events::nextAddr()
{
    if(m_list.isEmpty())
        return nullptr;

    return &m_list[0];
}

Event Events::at(int i)
{
    if(i<0 || m_list.isEmpty()||i>m_list.count())
       return Event{"",0};

    return m_list[i];
}

int Events::count()
{
    return m_list.count();
}

QList<Event> Events::list() const
{
    return m_list;
}

void Events::skipNext()
{
    m_list.removeFirst();
}

void Events::registerLastEvent(int secnds)
{
    if(m_list.isEmpty())
        return;

    Event e=next();

    if(e.hourIndex==0)
        return;

    QFile file(storageFile());
    if (!file.open(QIODevice::Append)) {

        return;
    }
    QTextStream out(&file);
    out<<"\n";
    out<<e.name+": ";
    out<<QDateTime::currentDateTime().toString();
    out << "("+QString::number(secnds)+"s)";

    file.close();


    return;
}

