#include "plantation.h"

Plantation::Plantation(QString variety, QString configName, QObject *parent)
    : QObject{parent},m_variety(variety)
{

}
