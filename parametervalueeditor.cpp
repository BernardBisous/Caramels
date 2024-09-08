#include "parametervalueeditor.h"
#include "qboxlayout.h"
#include "qlabel.h"

#define PURCENT_INC 5
#define BUTTONS_SIZE 50
ParameterValueEditor::ParameterValueEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr),
      m_mode(modeOffset),m_series(nullptr),
      m_gain(PURCENT_INC)
{
    setLayout(new QVBoxLayout);

  //  layout()->setContentsMargins(0,0,0,0);
    QWidget* modeWidget=new QWidget;
    modeWidget->setLayout(new QHBoxLayout);
    modeWidget->layout()->setContentsMargins(0,0,0,20);
    modeWidget->layout()->addWidget(m_value=new QLabel);
    modeWidget->layout()->addWidget(addMode(":/icons/offset","Offset"));
    modeWidget->layout()->addWidget(addMode(":/icons/gain","Gain"));

    QFont f=font();
    f.setPixelSize(16);
    m_value->setFont(f);

    layout()->addWidget(modeWidget);

    addMode(":/icons/Gain","Offset");



     QWidget*b=new QWidget;
     QGridLayout* gr=new QGridLayout;
     gr->setContentsMargins(0,0,0,0);
     gr->addWidget(m_up=new ToolButton("Up",":/icons/up"),0,1);
     gr->addWidget(m_left=new ToolButton("Left",":/icons/left"),1,0);
     gr->addWidget(m_right=new ToolButton("Right",":/icons/right"),1,2);
     gr->addWidget(m_down=new ToolButton("Down",":/icons/down"),2,1);

     b->setLayout(gr);

     layout()->addWidget(b);




     m_up->setRound(70);
     m_down->setRound(70);
     m_left->setRound(70);
     m_right->setRound(70);

    connect(m_up,SIGNAL(clicked()),this,SLOT(up()));
    connect(m_down,SIGNAL(clicked()),this,SLOT(down()));
    connect(m_left,SIGNAL(clicked()),this,SLOT(left()));
    connect(m_right,SIGNAL(clicked()),this,SLOT(right()));

    setMode(modeGain);
}

Parameter *ParameterValueEditor::client() const
{
    return m_client;
}

void ParameterValueEditor::setClient(Parameter *newClient)
{
    m_client = newClient;
    refreshValue();
}

void ParameterValueEditor::refresh()
{
    refreshMode();
}

ToolButton *ParameterValueEditor::addMode(QString icon, QString title)
{
    ToolButton*  b=new ToolButton(title,icon);
    m_modes.append(b);
    connect(b,SIGNAL(clicked()),this,SLOT(modeEdited()));
    b->setRound(BUTTONS_SIZE);
    return b;
}



ParameterValueEditor::Mode ParameterValueEditor::mode() const
{
    return m_mode;
}

void ParameterValueEditor::setMode(Mode newMode)
{
    qDebug()<<"settingMode"<<newMode;
    m_mode = newMode;
    refreshMode();
}

void ParameterValueEditor::refreshMode()
{
    for(int i=0;i<m_modes.count();i++)
    {
        if(m_mode==i)
            m_modes[i]->setMode(ActionWidget::highlighted);
        else
            m_modes[i]->setMode(ActionWidget::noBorder);
    }
}

void ParameterValueEditor::move(int inc)
{
    if(!m_client)
        return;

    qDebug()<<"move point";
    float val=(inc*m_gain)+100;
    val=val/100;
    if(m_mode==modeOffset)
    {
        float e=qAbs(m_client->maxY()-m_client->minY());
        val=inc*e*m_gain/100;
    }

    m_client->movePoints(m_indexes,val,m_mode==modeOffset);
    refreshValue();
    emit changed();
}

void ParameterValueEditor::edit(QList<int> indexes)
{
    m_indexes=indexes;

    refreshValue();
}

void ParameterValueEditor::edit(int i)
{
    m_indexes.clear();
    m_indexes.append(i);
    refreshValue();
}




void ParameterValueEditor::up()
{
    move(1);
}
void ParameterValueEditor::down()
{
     move(-1);

}
void ParameterValueEditor::left()
{
   selectOffset(-1);
}
void ParameterValueEditor::right()
{
    selectOffset(1);
}



void ParameterValueEditor::setSeries(QLineSeries *newSeries)
{
    m_series = newSeries;
    connect(m_series,SIGNAL(selectedPointsChanged()),this,SLOT(seriesSelected()));
}

void ParameterValueEditor::refreshValue()
{
    QString s;
    int n=lastSelected();
    if(n<0)
    {
        float max,min;
        m_client->rangeY(&max,&min);
         m_value->setText(QString::number(min)+"->"+QString::number(max)+m_client->units());
        return;
    }

    bool ok;
   float a=m_client->valueAtTime(n,&ok);
   if(!ok)
   {
       m_value->clear();
       return;
   }
    m_value->setText(QString::number(a)+m_client->units());
}

int ParameterValueEditor::lastSelected()
{
    if(!m_series || m_series->selectedPoints().isEmpty())
        return -1;

    return m_series->selectedPoints().last();
}

void ParameterValueEditor::setGain(float newGain)
{
    m_gain = newGain;
}

void ParameterValueEditor::selectOffset(int inc)
{
    QList<int> v,a;
    a=m_series->selectedPoints();

    if(a.isEmpty())
    {
        if(inc<0)
            m_series->selectPoint(m_series->count()-1);
        else
            m_series->selectPoint(0);
        return;
    }
    for(int i=0;i<a.count();i++)
    {
        int ia=a[i]+inc;

        if(ia<m_series->count() && ia>0)
            v<<ia;
    }
     disconnect(m_series,SIGNAL(selectedPointsChanged()),this,SLOT(seriesSelected()));
    m_series->deselectAllPoints();
    m_series->selectPoints(v);
     connect(m_series,SIGNAL(selectedPointsChanged()),this,SLOT(seriesSelected()));
     seriesSelected();

}

void ParameterValueEditor::modeEdited()
{

    for(int i=0;i<m_modes.count();i++)
        if(m_modes[i]==sender())
            setMode(Mode(i));

    refreshMode();
}

void ParameterValueEditor::seriesSelected()
{
    edit(m_series->selectedPoints());
}
