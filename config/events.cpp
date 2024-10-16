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
}

void Events::save(QDataStream &c)
{

    int n=m_list.count();
    c<<n;
    for(int i=0;i<m_list.count();i++)
    {

        c<<m_list[i].name;
        c<<m_list[i].hourIndex;
    }
}

bool Events::load(QDataStream &c)
{


    m_list.clear();
    int n;
    c>>n;
    for(int i=0;i<n;i++)
    {
        QString name;
        c>>name;
        int index;
        c>>index;
        add(name,index);


    }
    return true;
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

    qDebug()<<"saving "<<e.name<<secnds;

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
