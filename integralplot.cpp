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



    // Add the series to the chart
    initStyle();
 //   updatePlot();

    m_xAxis->setTitleVisible(false);
    setRenderHint(QPainter::Antialiasing);

    setMinimumSize(1,50);

    m_timer=new QTimer(this);
    m_timer->setInterval(200);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
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





    m_chart->setTitle("Total ["+c->integralUnit()+"]");
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



    if(!l.isEmpty() && (!minDate.isValid() || minDate>l.first().time))
        minDate=l.first().time;

    for(int i=0;i<l.count();i++)
    {
        float val=l[i].value;
        if(max<val )
            max=val;

        else if(min>val)
            min=val;


        s->append(l[i].time.toMSecsSinceEpoch(),val);

      //  if(i==l.count()-1)
      //       s->append(QDateTime::currentDateTime().toMSecsSinceEpoch(),val);
    }


    if(!l.isEmpty())
    {
        QString val=QString::number(l.last().value)+" "+c->integralUnit();
        s->setName(c->name()+": "+val);
    }

    s->append(QDateTime::currentDateTime().toMSecsSinceEpoch(),c->integral());
    m_chart->addSeries(s);




    s->attachAxis(m_xAxis);
    s->attachAxis(m_yAxis);
}

void IntegralPlot::setEnabledPlot(bool s)
{
    if(s)
        m_timer->start();
    else
        m_timer->stop();
}

void IntegralPlot::updatePlot()
{


    float min=0;
    float max=0;

    float maxDev=0;
    minDate=QDateTime();

    bool empty=true;
    for(int i=0;i<m_devices.count();i++)
    {

        updateSeries(m_devices[i],m_series[i]);

        float v=m_devices[i]->currentValue();
        if(v>0)
        {
            empty=false;
        }
    }

    m_xAxis->setRange(minDate,QDateTime::currentDateTime());
    m_yAxis->setRange(min,max);


    if(max!=0 &&  !empty)
    {
        setEnabledPlot(true);
    }
}

void IntegralPlot::timerSlot()
{
    updatePlot();
}
