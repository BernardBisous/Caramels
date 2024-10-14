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

#include <config/growconfig.h>



class ParameterPlot : public QWidget
{
    Q_OBJECT
public:
    ParameterPlot(QWidget*parent=nullptr);
//    bool eventFilter(QObject *obj, QEvent *event) override;
    void setParameter(Parameter*p);

    void setHighlight(bool s);
    void setXrange(int t);
    void initStyle();
    void select(int);
    void selectDefault();

    QLineSeries *series() const;


    Parameter *parameter() const;

    QDateTime startDate() const;
    void setStartDate(QDateTime newStartDate);

    int currentTimeIndex();
    int closerIndexPoint(int hourIndex);

    void mousePressEvent(QMouseEvent *event) override
        {
            if (event->button() == Qt::LeftButton) {
                // Emit the clicked signal
                emit clicked();
            }
            QWidget::mousePressEvent(event);
        }


    void refreshTitle();



public slots:
    void refresh();

signals:
    void clicked();

private:


    QChartView *m_view;
    QLineSeries *m_series;
    QValueAxis *m_xAxis;
    QValueAxis *m_yAxis;
    QChart *m_chart;
    Parameter *m_parameter;

    QDateTime m_startDate;

    QGraphicsItem* m_selectedItem;


};



#endif // PARAMETERPLOT_H
