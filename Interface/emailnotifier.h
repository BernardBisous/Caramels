#ifndef EMAILNOTIFIER_H
#define EMAILNOTIFIER_H

#include <QObject>

class EmailNotifier : public QObject
{
    Q_OBJECT
public:
    typedef enum topic{Issue,News,Task}Topic;

    static void send(QString text,Topic c=Issue);
    explicit EmailNotifier(QObject *parent = nullptr);



};

#endif // EMAILNOTIFIER_H
