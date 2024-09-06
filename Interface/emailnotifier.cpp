#include "emailnotifier.h"
#include "SMTP/mimehtml.h"
#include "SMTP/smtpclient.h"
#include "SMTP/mimemessage.h"
#include "SMTP/smtpmime_global.h"
#include "SMTP/emailaddress.h"
#include "SMTP/mimehtml.h"
#include "SMTP/mimeinlinefile.h"
#include "qdebug.h"



#define SENDER "adrien.girod@fleche.tech"
#define SENDER_NAME "Adrien"
#define SUBJECT "Your reward is ready"

#define SERVER "mail.infomaniak.com"
#define PORT 465

#define LOGIN "adrien.girod@fleche.tech"
#define PASSWORD "lion.OL@74160"


#include <QFile>
EmailNotifier::EmailNotifier(QObject *parent)
    : QObject{parent}
{
    m_server=new SmtpClient(SERVER,PORT,SmtpClient::SslConnection);
    connectToServer();
}

void EmailNotifier::send(QString subject, QString body, Topic c)
{
    MimeMessage message;
    EmailAddress sender(SENDER,SENDER_NAME);
    message.setSender(sender);

    QList<EmailAddress> r=recipients();
    for(int i=0;i<r.count();i++)
         message.addRecipient(r[i]);

    message.setSubject(subject);

    MimeHtml html;
    QString st=htmlContent(c).arg(body);
    html.setHtml(st);
    message.addPart(&html);

    m_server->sendMail(message);
}

int EmailNotifier::connectToServer()
{

    qDebug()<<"connecting server"<<SERVER<<PORT;
   m_server->connectToHost();

   if(m_server)
    qDebug()<<"connected to server"<<SERVER<<PORT;
   if (!m_server->waitForReadyConnected(50)) {
       console("Failed to connect to host!");
       return -1;
   }

    qDebug()<<"loggin server"<<LOGIN<<PASSWORD;
   m_server->login(LOGIN,PASSWORD);
   if (!m_server->waitForAuthenticated()) {
       console("Failed to login!");
       return -2;
   }

   console("connected");
   return 1;
}

QList<EmailAddress> EmailNotifier::recipients()
{
    QList<EmailAddress> out;
    auto l=emailAddressesStr();
    for(int i=0;i<l.count();i++)
    {
       EmailAddress a(l[i],"Garde de la tour");
       out.append(a);
    }
    return out;
}

void EmailNotifier::console(QString s)
{
    qDebug()<<"email console"<<s;
}

QString EmailNotifier::htmlContent(Topic )
{

    QFile file(":/mails/issue");
    if(!file.open(QIODevice::Text | QIODevice::ReadOnly))
     {
         console("cannot load mail htmlfile");
         return "";
     }

     QTextStream s(&file);
     return s.readAll();

}


void EmailNotifier::sentSlot()
{

}

QStringList EmailNotifier::emailAddressesStr()
{
    return QStringList()<<"adrien.bernard.girod@gmail.com";
}
