#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Interface/events.h"
#include "Interface/toolbutton.h"
#include "qdatetime.h"
#include "qlabel.h"
#include <QWidget>
#include "tent.h"
class EventManager : public QWidget
{
    Q_OBJECT
public:
    explicit EventManager(QWidget *parent = nullptr);
    void handle(Events* e,Tent* t);
    void refresh();
    void showAll(bool s);


    QDateTime startedDate() const;

public slots:
    void setStartedDate(QDateTime newStartedDate);
    void skip();
    void start();
    void cancel();



private :

    Tent* m_tent;
    bool m_abstracted;
    QDateTime m_eventStart;
    QDateTime m_startedDate;
    Events*m_client;
    ToolButton* m_startButton;
    ToolButton* m_skipButton;
    QLabel* m_nameLabel;
    QLabel* m_dateLabel;
    QLabel* m_listLabel;
    QWidget* m_emptyLabel;
    ActionWidget* m_editor;
};

#endif // EVENTMANAGER_H
