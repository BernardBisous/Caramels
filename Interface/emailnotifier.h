#ifndef EMAILNOTIFIER_H
#define EMAILNOTIFIER_H

#include <QObject>

class EmailNotifier : public QObject
{
    Q_OBJECT
public:
    typedef enum topic{Issue,News,Task}Topic;
    static void send(QString text,Topic c=Issue);
    static void error(QString text);

    explicit EmailNotifier(QObject *parent = nullptr);
    void send(QString text,Topic c,bool del);
    bool m_del;
};

#endif // EMAILNOTIFIER_H
