#ifndef EVENTWIZZARD_H
#define EVENTWIZZARD_H

#include "config/events.h"
#include "hardware/tent.h"
#include "qlineedit.h"
#include "qtextedit.h"
#include "widgets/webcamwidget.h"
#include "wizzard.h"
#include <QWidget>


class EventWizzard : public Wizzard
{
    Q_OBJECT
public:
    explicit EventWizzard(Tent* t,Event* e,QWidget*parent=nullptr);
    static EventWizzard *executeEvent(Tent* t,Event* e);
    void loadEventName(QString name);

signals:
private:
    Event* m_client;
    Tent* m_tente;

};


class ArchiveSequence : public WizzardSequence
{
    Q_OBJECT
public:
    explicit ArchiveSequence(Tent* e,QWidget*parent=nullptr);

private:
    Tent* m_tente;
    QLineEdit* m_name;
    WebcamWidget* m_webcam;
    QTextEdit* m_comments;

};


#endif // EVENTWIZZARD_H
