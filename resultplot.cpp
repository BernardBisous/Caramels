#include "resultplot.h"
#include "qboxlayout.h"

/*
ResultPlot::ResultPlot(QWidget *parent)
    :    QChartView(parent), m_tent(nullptr),
      m_key(),m_chart(new QChart()),m_series(new QLineSeries())
{
    setChart(m_chart);
    m_xAxis = new QDateTimeAxis;
    m_xAxis->setFormat("hh:mm:ss");
    m_xAxis->setTitleText("Time");
    m_yAxis = new QValueAxis;

    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);

    // Add the series to the chart





    initStyle();
 //   updatePlot();

    setRenderHint(QPainter::Antialiasing);
}

void ResultPlot::handle(Tent *t, QString key)
{
    m_tent=t;
    m_key=key;
    if(t)
    {
        m_keyIndex=t->resultKeys().indexOf(key);
        connect(t,SIGNAL(newValue(int)),this,SLOT(updatePlot()));
    }


    m_chart->setTitle(key);
    updatePlot();
}

void ResultPlot::updatePlot()
{
    chart()->removeSeries(m_series);
    m_series->clear();

    if(!m_tent)
        return;


    float max=0;
    auto l=m_tent->resultsFor(m_key);
    //
    for(int i=0;i<l.size();i++)
    {
        float val=l[i].value;

       // val=i;
        if(!i || max<val)
            max=val;

       // qDebug()<<"uupdateTtt"<<val<<l[i].time.toMSecsSinceEpoch();
        m_series->append(l[i].time.toMSecsSinceEpoch(),val);
    }

   // qDebug()<<"uupdateTentrest"<<l.count()<<max;
    m_chart->addSeries(m_series);
   // m_series->attachAxis(m_xAxis);
   // m_series->attachAxis(m_yAxis);

    m_yAxis->setRange(-1,max*1.05);

    if(l.count())
        m_xAxis->setRange(l.first().time,QDateTime::currentDateTime());
}

void ResultPlot::initStyle()
{
    QColor gridColor(palette().alternateBase().color());
    QColor textColor(palette().text().color());

    m_chart->setBackgroundBrush(palette().base());
    m_series->setColor(palette().highlight().color());
    m_chart->setPlotAreaBackgroundPen(QPen(Qt::yellow));

    m_xAxis->setGridLineColor(gridColor);
    m_xAxis->setLinePenColor(gridColor);
    m_xAxis->setLabelsColor(textColor);
    m_xAxis->setTitleBrush(textColor);

    m_yAxis->setGridLineColor(gridColor);
    m_yAxis->setLinePenColor(gridColor);
    m_yAxis->setLabelsColor(textColor);
    m_yAxis->setTitleBrush(textColor);
    m_chart->setBackgroundRoundness(20);

    m_chart->setTitleBrush(textColor);
    m_chart->legend()->hide();
    m_chart->update();
}

ResultsWidget::ResultsWidget(QWidget *parent)
    :QWidget(parent)
{
    setLayout(new QVBoxLayout);

}

void ResultsWidget::handle(Tent *t)
{
    for(int i=0;i<t->resultKeys().count();i++)
    {
        ResultPlot*p=new ResultPlot;
        m_plots.append(p);
        layout()->addWidget(p);

        if(t && i>=0 && i<t->resultKeys().count())
            p->handle(t,t->resultKeys()[i]);

    }
}
*/
