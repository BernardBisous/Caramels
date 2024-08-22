#ifndef PARAMETERLISTWIDGET_H
#define PARAMETERLISTWIDGET_H

#include "Interface/actionwidget.h"
#include "Interface/scrollarea.h"
#include "growconfig.h"
#include <QWidget>
#include <QScrollArea>

class ParameterListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterListWidget(QWidget *parent = nullptr);
    void handle(GrowConfig* c);
    void setSelected(int i);
    void refreshList();
    int currentSelected();
    QList<ActionWidget*> items();



signals:
    void selected(int i);
    void addOne();

private slots:
    void trigSlot(int,QWidget*);
    void addSlot(){emit addOne();}


private:
    ActionWidget* m_add;
    ScrollArea* m_list;
    GrowConfig* m_client;

};

#endif // PARAMETERLISTWIDGET_H
