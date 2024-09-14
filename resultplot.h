#ifndef RESULTPLOT_H
#define RESULTPLOT_H

#include "qchartview.h"
#include "qdatetimeaxis.h"
#include "qlineseries.h"
#include "qvalueaxis.h"
#include "tent.h"
#include <QWidget>

/*
class ResultPlot : public QChartView
{
    Q_OBJECT
public:
    explicit ResultPlot(QWidget *parent = nullptr);
    void handle(Tent* t, QString key);
    void initStyle();
public slots:

    void updatePlot() ;


signals:

private:
    Tent* m_tent;
    QString m_key;
    int m_keyIndex;

    QChart* m_chart;
    QLineSeries* m_series;
    QDateTimeAxis *m_xAxis;
    QValueAxis *m_yAxis;

};


class ResultsWidget:public QWidget
{

    Q_OBJECT
public:
    explicit ResultsWidget(QWidget *parent = nullptr);
    void handle(Tent* t);


private:
    QList<ResultPlot*> m_plots;


};
*/
#endif // RESULTPLOT_H
