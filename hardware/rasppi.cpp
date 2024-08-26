#include "rasppi.h"
#include "qdebug.h"
#include <QRandomGenerator>


RasPi* m_instance;


RasPi::RasPi(QObject *parent):QObject(parent)
{

}

void RasPi::initPin(int pin, PinMode m)
{
   // qDebug()<<"raspi init"<<pin<<m;
}

void RasPi::write(int pin, int purcent)
{
    //qDebug()<<"raspi write"<<pin<<purcent;
}

bool RasPi::readHigh(int )
{
    return QRandomGenerator::global()->generate() % 2;



}

int RasPi::readAnalog(int)
{
    return QRandomGenerator::global()->generate() % 100;
}

void RasPi::begin()
{

}
