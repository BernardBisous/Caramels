#include "integralplot.h"

IntegralPlot::IntegralPlot(QWidget *parent)
    : QChartView{parent},m_chart(new QChart())
{
    setContentsMargins(0,0,0,0);

    m_chart->legend()->setVisible(false);
    setChart(m_chart);
    m_chart->layout()->setContentsMargins(0,0,0,0);


    // Create the axes
    m_xAxis = new QDateTimeAxis;
    m_xAxis->setFormat("hh:mm:ss");
    m_xAxis->setTitleText("Time");
    m_yAxis = new QValueAxis;

    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);



    m_chart->setTitle("Total injected");
    // Add the series to the chart
    initStyle();
 //   updatePlot();

    m_xAxis->setTitleVisible(false);
    setRenderHint(QPainter::Antialiasing);
}

void IntegralPlot::handle(Actuator *c)
{

    removeAll();
    if(!c)
        return;


    attach(c);

    m_chart->legend()->hide();
    updatePlot();


}

void IntegralPlot::handle(QList<Actuator *> s)
{
    removeAll();
    for(int i=0;i<s.count();i++)
    {
        attach(s[i]);
    }
;

    initStyle();
    updatePlot();

    m_chart->legend()->setVisible(s.count()>1);

}

void IntegralPlot::attach(Actuator *c)
{

    connect(c, &Device::newValue, this, &IntegralPlot::updatePlot);

    QLineSeries* s=new QLineSeries;
    m_series.append(s);
    m_devices.append(c);

    s->setName(c->name());




    m_chart->setTitle("Total injected ["+c->integralUnit()+"]");
    initStyle();
    updatePlot();

}
void IntegralPlot::initStyle()
{

    QColor gridColor(palette().alternateBase().color());
    QColor textColor(palette().text().color());

    m_chart->setBackgroundBrush(palette().base());
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
    m_chart->update();
}

void IntegralPlot::removeAll()
{
    for(int i=0;i<m_devices.count();i++)
        disconnect(m_devices[i], &Device::newValue, this, &IntegralPlot::updatePlot);

    for(int i=0;i<m_series.count();i++)
    {
        m_chart->removeSeries(m_series[i]);
        delete m_series[i];
    }



    m_series.clear();
    m_devices.clear();
}

void IntegralPlot::updateSeries(Actuator *c, QLineSeries *s)
{
    if(m_chart->series().contains(s))
        chart()->removeSeries(s);

    s->clear();

    if(!c)
        return;

    auto l=c->integralHistoric();

    QDateTime sh;
    if(!l.isEmpty())
        sh=l.first().time;

    float max=0;
    QDateTime minDate;
    if(!l.isEmpty())
        minDate=l.first().time;
    else
        minDate=QDateTime::currentDateTime().addSecs(-10);


    for(int i=0;i<l.count();i++)
    {
        if(l[i].time>minDate)
        {
            if(!i|| max<l[i].value )
                max=l[i].value;


            s->append(l[i].time.toMSecsSinceEpoch(),l[i].value);
        }
    }


    if(!l.isEmpty())
    {
        QString val=QString::number(l.last().value)+" "+c->integralUnit();
        s->setName(c->name()+": "+val);
    }

    s->append(QDateTime::currentDateTime().toMSecsSinceEpoch(),c->integral());
    m_chart->addSeries(s);

    m_xAxis->setRange(minDate,QDateTime::currentDateTime().addSecs(1));
    m_yAxis->setMax(max);
    m_yAxis->setMin(0);

    s->attachAxis(m_xAxis);
    s->attachAxis(m_yAxis);
}

void IntegralPlot::setEnabledPlot(bool s)
{
    qDebug()<<"enable integrals to implement";
}

void IntegralPlot::updatePlot()
{
    for(int i=0;i<m_devices.count();i++)
        updateSeries(m_devices[i],m_series[i]);
}
