#ifndef EMAILNOTIFIER_H
#define EMAILNOTIFIER_H

#include "SMTP/smtpclient.h"
#include "statenotifier.h"
#include <QObject>

class EmailNotifier : public QObject
{
    Q_OBJECT
public:
    typedef enum topic{Issue,News,Task,Startup}Topic;
    static void send(QString text,Topic c=Issue);
    static void error(StateNotifier *s);
    static void starts(StateNotifier* s, QString ps="");
    static QString loadEmail(Topic c, QStringList args);


    explicit EmailNotifier(QObject *parent = nullptr);
    ~EmailNotifier();

    void send(QString text,Topic c,bool del);

    bool m_del;

private:
    SmtpClient* m_server;
    bool m_ok;
};

#endif // EMAILNOTIFIER_H
