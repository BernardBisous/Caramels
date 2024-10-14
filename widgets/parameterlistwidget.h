#ifndef PARAMETERLISTWIDGET_H
#define PARAMETERLISTWIDGET_H

#include "Interface/actionwidget.h"
#include "Interface/scrollarea.h"
#include "config/growconfig.h"
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



    QList<Parameter *> params() const;
    void setParams(const QList<Parameter *> &newParams);
    QStringList paramNames();

    int current() const;
    void setCurrent(int newCurrent);

signals:
    void selected(int i,Parameter*p);
    void addOne();

private slots:
    void trigSlot(int,QWidget*);
    void addSlot(){emit addOne();}


private:
    int m_current;
    ActionWidget* m_add;
    QList<Parameter*>m_params;
    ScrollArea* m_list;


};

#endif // PARAMETERLISTWIDGET_H
