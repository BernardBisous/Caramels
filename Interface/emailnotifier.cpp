#include "emailnotifier.h"
#include "SMTP/mimehtml.h"
#include "SMTP/mimetext.h"
#include "qdebug.h"
#include "qprocess.h"
#include <QFile>


#define SERVER "smtp.gmail.com"
#define PORT 465
#define PASSWORD SMTP_PASS
#define LOGIN "sublime.biennoise@gmail.com"

#define SENDER LOGIN
#define SENDER_NAME "Tente Sublime"

void EmailNotifier::send(QString text, Topic c)
{
   auto e=new EmailNotifier;
   e->send(text,c,true);
}

void EmailNotifier::error(StateNotifier *s)
{
    auto l=s->bads();
    QStringList out;
    for(int i=0;i<l.count();i++)
    {
        out<<l[i]->fullDiagnosis();
    }

    QStringList st;
    st<<"J'ai une erreur hardware";

    if(out.isEmpty())
        st<<"Tout ok 👌";
    else
        st<<out.join("</p>\n<p");



    QString sep=loadEmail(Issue,st);
    send(sep,Issue);
}

void EmailNotifier::starts(StateNotifier *s, QString ps)
{
    auto l=s->bads();
    QStringList out;
    for(int i=0;i<l.count();i++)
    {
        out<<l[i]->fullDiagnosis();
    }

    QStringList st;
    st<<"La tente est maintenant disponible";

    if(out.isEmpty())
        st<<"Tout ok 👌";
    else
        st<<out.join("</p>\n<p");

    st<<ps;

    QString sep=loadEmail(Startup,st);
    send(sep,Startup);
}

QString EmailNotifier::loadEmail(Topic c,QStringList args)
{
    QString file;
    switch (c)
    {
    case Startup: file=":/mails/status";break;
    default : file=":/mails/error";break;
    }

    QFile f(":/mails/status");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QString se=f.readAll();

    for(int i=0;i<args.count();i++)
    {
        se.replace("%"+QString::number(i+1),args[i]);
    }
    return se;
}

EmailNotifier::EmailNotifier(QObject *parent)
    : QObject{parent},m_ok(true)
{
    m_server=new SmtpClient(SERVER,PORT,SmtpClient::SslConnection);
    m_server->connectToHost();
    if (!m_server->waitForReadyConnected(100)) {
        qDebug("Failed to connect to host!");
        m_ok=false;
    }

    m_server->login(LOGIN,qgetenv("SMTP_PASS"));

    if (!m_server->waitForAuthenticated(500)) {
        qDebug("Failed to login!");
         m_ok=false;
    }

}

EmailNotifier::~EmailNotifier()
{
    delete m_server;

}

void EmailNotifier::send(QString text, Topic c, bool del)
{

    MimeMessage message;
    EmailAddress sender(SENDER,SENDER_NAME);
    message.setSender(sender);

  //  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString s= qgetenv("EMAILS");
    QStringList ls=s.split(";");

    qDebug()<<"loaded mails"<<ls;
    for(int i=0;i<ls.count();i++)
    {
        qDebug()<<"adding email receiver"<<ls[i];
        EmailAddress rec(ls[i],SENDER_NAME);
        message.addRecipient(rec);
    }


    MimeHtml th;
    th.setText(text);
    message.addPart(&th);

    QString topic;
    switch (c)
    {
        case Issue:topic="Problème détecté";break;
        case News:topic="Précision";break;
        case Task:topic="Tache prévue";break;
        case Startup:topic="Tente allumée";break;
    }
    message.setSubject(topic);

    m_server->sendMail(message);
    if (!m_server->waitForMailSent(3000)) {
        qDebug( "❌Failed to send to ");
    }


    else
        qDebug()<< "✅message sent ";

    if(del)
        delete this;
}
