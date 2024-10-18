#ifndef STATSWIDGET_H
#define STATSWIDGET_H

#include "Interface/progressbar.h"
#include "qformlayout.h"
#include <QWidget>

class StatsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatsWidget(QWidget *parent = nullptr);
    void setHash(QHash<QString,int>);


private:
    QFormLayout* m_form;
    QList<ProgressBar*> m_list;

};

#endif // STATSWIDGET_H
