#ifndef EMAILNOTIFIER_H
#define EMAILNOTIFIER_H

#include "SMTP/smtpclient.h"

#include <QObject>

class EmailNotifier : public QObject
{
    Q_OBJECT
public:
    typedef enum topic{Issue,News,Task}Topic;

    explicit EmailNotifier(QObject *parent = nullptr);
    void send(QString subject, QString body,Topic c=Issue);
    int connectToServer();
    QStringList emailAddressesStr();
    QList<EmailAddress> recipients();
    void console(QString s);

    QString htmlContent(Topic c);


private slots:
    void sentSlot();
signals:
    void finished(QString error);

private:
    SmtpClient* m_server;


};

#endif // EMAILNOTIFIER_H
