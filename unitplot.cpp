#include "unitplot.h"
#include <qsplineseries.h>

UnitPlot::UnitPlot(QWidget *parent): QWidget(parent),m_client(nullptr)
{

    m_timer=new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateSensors()));

    m_view = new QChartView(this);
    m_view->setRenderHint(QPainter::Antialiasing);


    // Create axes
    m_xAxis = new QDateTimeAxis;
    m_xAxis->setFormat("hh:mm:ss");
    m_xAxis->setTitleText("Time");
    m_yAxis = new QValueAxis;
    m_xAxis->setTitleVisible(true);

    m_yAxis->setRange(0,100);

    // Set axis properties




    // Create the chart and add the series and axes
    m_chart = new QChart();
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);


    m_view->setChart(m_chart);
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(m_view);



    setMinimumSize(200,200);
    initStyle();

}

void UnitPlot::removeMargins()
{
    layout()->setContentsMargins(0,0,0,0);

    m_view->setContentsMargins(0,0,0,0);
}

void UnitPlot::handle(HardwareUnit *u)
{
    initStyle();
    m_startDate=u->startTime();
    m_client=u;

    m_chart->setTitle(u->name());



    for(int i=0;i<m_devicesSeries.count();i++)
        delete m_devicesSeries[i];
    m_devicesSeries.clear();

    QPen pen(palette().highlight().color());
    pen.setStyle(Qt::DotLine);
    for(int i=0;i<u->devices().count();i++)
    {
        QLineSeries * s=new QLineSeries;
        m_devicesSeries.append(s);
        s->setPen(pen);

        m_chart->addSeries(s);
        s->attachAxis(m_xAxis);
        s->attachAxis(m_yAxis);
    }


    for(int i=0;i<m_parametersSeries.count();i++)
        delete m_parametersSeries[i];
    m_parametersSeries.clear();

    pen.setStyle(Qt::SolidLine);
    for(int i=0;i<u->parameters().count();i++)
    {
        QLineSeries* s=new QLineSeries;
        m_parametersSeries.append(s);
        s->setPointsVisible();
        m_chart->addSeries(s);
        s->attachAxis(m_xAxis);
        s->attachAxis(m_yAxis);
    }

    updatePlot();

    if(!m_devicesSeries.isEmpty())
        m_timer->start();
}

void UnitPlot::updateParameters()
{
    m_startDate=m_client->startTime();

    for(int j=0;j<m_client->parameters().count();j++)
    {
        updateParameter(m_parametersSeries[j],m_client->parameters()[j],m_startDate);
    }
    updateScale();
}

void UnitPlot::updateScale()
{
    m_yAxis->setMax(110);
    m_yAxis->setMin(-10);
    QDateTime endConfig=m_client->endConfig();
    if(m_startDate.isValid())
    {
         m_xAxis->setRange(m_startDate,endConfig);
    }
    else m_xAxis->setMax(endConfig);

}

void UnitPlot::updatePlot()
{
    updateSensors();
    updateParameters();
}

void UnitPlot::updateSensors()
{
    int i=0;

    initStyle();

    for(i=0;i<m_client->devices().count();i++)
    {
        updateSensor(m_devicesSeries[i],m_client->devices()[i]);
    }

    if(m_client->parameters().isEmpty())
    {
        return;
    }


    updateScale();
}

Device *UnitPlot::targeting() const
{
    return m_targeting;
}

void UnitPlot::setTargeting(Device *newTargeting)
{
    m_targeting = newTargeting;
}



HardwareUnit *UnitPlot::client() const
{
    return m_client;
}

void UnitPlot::setStartDate(const QDateTime &newStartDate)
{
    m_startDate = newStartDate;

}

QLineSeries *UnitPlot::paramSerie(Parameter *p)
{

    int index=m_client->parameters().indexOf(p);
    if(index>=0)
    {
        return m_parametersSeries[index];
    }
    return nullptr;
}





QDateTime UnitPlot::endConfig()
{
    if(!m_client)
        return QDateTime::currentDateTime();
    return m_client->endConfig();
}

void UnitPlot::updateSensor(QLineSeries *series, Device *s)
{
    m_chart->removeSeries(series);
    series->clear();
    int size=200;
    QList<RealTimeValue> in=s->values();
    int n=in.count();
    int i=n-size-1;

    if(i<0)
        i=0;


    auto l=s->values();
    auto lh=s->historic();

    QDateTime sh;
    if(!l.isEmpty())
        sh=l.first().time;

    float max=0;
    QDateTime minDate;
    if(!lh.isEmpty())
        minDate=lh.first().time;
    else if(!l.isEmpty())
        minDate=l.first().time;
    else
        minDate=QDateTime::currentDateTime().addSecs(-10);

    for(int i=0;i<lh.count();i++)
    {
        if(sh.isValid() && lh[i].time<sh)
        {
            if( lh[i].time>minDate)
            {
                if(!i || max<lh[i].value)
                    max=lh[i].value;

                series->append(lh[i].time.toMSecsSinceEpoch(),s->mapToPurcent(lh[i].value));
            }
        }
    }

    for(int i=0;i<l.count();i++)
    {
        if(l[i].time>minDate)
        {
            if((!i&&lh.isEmpty()) || max<l[i].value)
                max=l[i].value;


            series->append(l[i].time.toMSecsSinceEpoch(),s->mapToPurcent(l[i].value));
        }
    }





    if(series->count()<=1)
    {
       series->append(minDate.toMSecsSinceEpoch(),s->currentValue());
    }

    series->append(QDateTime::currentDateTime().toMSecsSinceEpoch(),s->currentValue());

    m_chart->addSeries(series);

    QDateTime tf=endConfig();

    if(minDate<m_startDate || !m_startDate.isValid())
        m_xAxis->setRange(minDate,tf.addSecs(1));
    else
         m_xAxis->setRange(m_startDate,tf.addSecs(1));

    series->attachAxis(m_xAxis);
    series->attachAxis(m_yAxis);
}

void UnitPlot::updateParameter(QLineSeries *l, Parameter *p,QDateTime start)
{
    m_chart->removeSeries(l);
    l->clear();
    int n=p->count();
    for(int i=0 ; i<n ;i++)
    {
        auto v=p->at(i);
        QDateTime te=start.addSecs(v.hourIndex*Parameter::timeMultiplicator());

        l->append(te.toMSecsSinceEpoch(),v.value);
    }


    m_chart->addSeries(l);
    l->attachAxis(m_xAxis);
    l->attachAxis(m_yAxis);
}

void UnitPlot::initStyle()
{

    QColor gridColor(palette().alternateBase().color());
    QColor textColor(palette().text().color());

    m_chart->setBackgroundBrush(palette().base());
   // m_series->setColor(palette().highlight().color());
    //m_chart->setPlotAreaBackgroundPen(QPen(Qt::yellow));

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
    m_view->update();
    m_view->repaint();
    repaint();
    update();


}

float UnitPlot::maxY()
{
    return 100;
}
