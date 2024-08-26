#ifndef PARAMETERLISTWIDGET_H
#define PARAMETERLISTWIDGET_H

#include "Interface/actionwidget.h"
#include "Interface/scrollarea.h"
#include "growconfig.h"
#include "hardware/hardwareunit.h"
#include <QWidget>
#include <QScrollArea>

class ParameterListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterListWidget(QWidget *parent = nullptr);
    void handle(GrowConfig* c);
    void handleHadware(HardwareUnit* h);
    void setSelected(int i);
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


};

#endif // PARAMETERLISTWIDGET_H
