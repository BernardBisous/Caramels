#include "tent.h"

Tent::Tent(QObject *parent)
    : QObject{parent},m_config(nullptr)
{

}

QString Tent::configName() const
{
    return m_configName;
}

void Tent::setConfig(GrowConfig *e)
{
    m_config=e;
}


void Tent::restart()
{
    m_startedDate=QDateTime::currentDateTime();
}

GrowConfig *Tent::config() const
{
    return m_config;
}
