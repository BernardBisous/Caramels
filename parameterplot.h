#ifndef PARAMETERPLOT_H
#define PARAMETERPLOT_H

#include "qscatterseries.h"
#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QGridLayout>

#include <growconfig.h>





class ParameterPlot : public QWidget
{
    Q_OBJECT
public:
    ParameterPlot(QWidget*parent=nullptr);
    bool eventFilter(QObject *obj, QEvent *event) override;
    void refresh(Parameter*p);
    void setXrange(int t);
    void initStyle();
    void select(int);

signals:
    void clicked(QPointF p);

private:



    QChartView *m_view;
    QLineSeries *m_series;
    QValueAxis *m_xAxis;
    QValueAxis *m_yAxis;
    QChart *m_chart;
    Parameter *m_parameter;
};



#endif // PARAMETERPLOT_H
