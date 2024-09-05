#include "unitplot.h"
#include <qsplineseries.h>

UnitPlot::UnitPlot(QWidget *parent): QWidget(parent),m_client(nullptr)
{

    m_timer=new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updatePlot()));

    m_view = new QChartView(this);
    m_view->setRenderHint(QPainter::Antialiasing);


    // Create axes
    m_xAxis = new QDateTimeAxis;
    m_xAxis->setFormat("hh:mm:ss");
    m_xAxis->setTitleText("Time");
    m_yAxis = new QValueAxis;
    m_xAxis->setTitleVisible(true);

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

void UnitPlot::handle(HardwareUnit *u, QDateTime t)
{
    for(int i=0;i<m_series.count();i++)
        delete m_series[i];
    m_series.clear();

    m_startDate=t;
    m_client=u;

    m_chart->setTitle(u->name());

    QPen pen(palette().highlight().color());
    pen.setStyle(Qt::DotLine);
    for(int i=0;i<u->devices().count();i++)
    {
        QLineSeries * s=new QLineSeries;
        m_series.append(s);
        s->setPen(pen);

        m_chart->addSeries(s);
        s->attachAxis(m_xAxis);
    }


    pen.setStyle(Qt::SolidLine);
    for(int i=0;i<u->parameters().count();i++)
    {
        QLineSeries* s=new QLineSeries;
        m_series.append(s);
        s->setPointsVisible();
        m_chart->addSeries(s);
        s->attachAxis(m_xAxis);
    }



    initStyle();

    updatePlot();
    m_timer->start();
}

void UnitPlot::updatePlot()
{
    int i=0;


    int maxX=0;
     initStyle();

    for(i=0;i<m_client->devices().count();i++)
    {
        updateSensor(m_series[i],m_client->devices()[i]);
    }

    if(m_client->parameters().isEmpty())
    {
        return;
    }


    for(int j=0;j<m_client->parameters().count();j++)
    {

        int m=m_client->parameters()[j]->maxX();
        if(m>maxX)
            maxX=m;

        updateParameter(m_series[i+j],m_client->parameters()[j],m_startDate);
    }


    m_xAxis->setRange(m_startDate,m_startDate.addSecs(maxX*Parameter::timeMultiplicator()));
}

void UnitPlot::setStartDate(const QDateTime &newStartDate)
{
    m_startDate = newStartDate;
    updatePlot();
}

void UnitPlot::updateSensor(QLineSeries *l, Device *s)
{
    m_chart->removeSeries(l);
    l->clear();
    int size=200;
    QList<RealTimeValue> in=s->values();
    int n=in.count();
    int i=n-size-1;
    if(i<0)
        i=0;
    for(;i<n;i++)
    {
        l->append(in[i].time.toMSecsSinceEpoch(),in[i].value);
    }

    m_chart->addSeries(l);
    l->attachAxis(m_xAxis);
}

void UnitPlot::updateParameter(QLineSeries *l, Parameter *p,QDateTime start)
{
    m_chart->removeSeries(l);
    l->clear();
    int n=p->count();
    for(int i=0 ; i<n ;i++)
    {
        auto v=p->at(i);
        QDateTime te=start.addSecs(v.x());

        l->append(te.toMSecsSinceEpoch(),v.y());
    }


    m_chart->addSeries(l);
    l->attachAxis(m_xAxis);
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
