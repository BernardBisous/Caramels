#include "emailnotifier.h"
#include "qdebug.h"
#include <QFile>


void EmailNotifier::send(QString text, Topic c)
{
   auto e=new EmailNotifier;
   e->send(text,c,true);
}

void EmailNotifier::error(QString text)
{
    send(text,Issue);
}

EmailNotifier::EmailNotifier(QObject *parent)
    : QObject{parent}
{

}

void EmailNotifier::send(QString text, Topic c, bool del)
{
    qDebug()<<"sending email to implement"<<c<<text<<del;
    delete this;
}
