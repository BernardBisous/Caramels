#include "plantation.h"
#include <QFile>

#define DEFAULT_PATH "runs"
Plantation::Plantation(QObject *parent)
    : QObject{parent},m_config(nullptr),m_configPath("")
{
        m_timer=new QTimer(this);
        connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
        m_timer->setInterval(Parameter::timeMultiplicator());

        load(DEFAULT_PATH);

}

bool Plantation::load(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        // Gérer l'erreur si le fichier ne peut pas être ouvert
       // qDebug()<<"cannot open config file "<<filename;

        return false;
    }

    QDataStream in(&file);
    bool c= load(in);
    file.close();
    return c;
}

bool Plantation::load(QDataStream & c)
{
    if(c.atEnd())
        return false;

   c>>m_configPath;
   c>>m_startDate;

   return true;
}

void Plantation::save(QDataStream &c)
{
    c<<m_configPath;
    c<<m_startDate;
}

void Plantation::start(GrowConfig *c)
{

    m_config=c;
    m_startDate=QDateTime::currentDateTime();
     QFile file(DEFAULT_PATH);
     if (!file.open(QIODevice::Truncate)) {
         // Gérer l'erreur si le fichier ne peut pas être ouvert
        // qDebug()<<"cannot open config file "<<filename;

         return;
     }

     QDataStream in(&file);
     save(in);
     file.close();
}



void Plantation::begin()
{
    timerSlot();
    m_timer->start();
}

void Plantation::timerSlot()
{
    int h=m_startDate.secsTo(QDateTime::currentDateTime())/60;
    emit configSample(h);
}

QDateTime Plantation::startDate() const
{
    return m_startDate;
}

GrowConfig *Plantation::config() const
{
    return m_config;
}
