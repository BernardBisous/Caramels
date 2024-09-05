#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Interface/events.h"
#include "Interface/toolbutton.h"
#include "qdatetime.h"
#include "qlabel.h"
#include <QWidget>

class EventManager : public QWidget
{
    Q_OBJECT
public:
    explicit EventManager(QWidget *parent = nullptr);
    void handle(Events* e);
    void refresh();




    QDateTime startedDate() const;

public slots:
    void setStartedDate(QDateTime newStartedDate);
    void skip();
    void start();
    void cancel();
signals:

private :

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
