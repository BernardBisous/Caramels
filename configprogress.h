#ifndef CONFIGPROGRESS_H
#define CONFIGPROGRESS_H

#include "Interface/progressbar.h"
#include "growconfig.h"
#include "qlabel.h"
#include "tent.h"
#include <QWidget>

class ConfigProgress : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigProgress(QWidget *parent = nullptr);
    void refresh(GrowConfig* c,QDateTime startDate);
    void refresh(Tent*t);
signals:


private:
    QLabel* m_start;
    QLabel* m_end;
    ProgressBar* m_progress;
};

#endif // CONFIGPROGRESS_H
