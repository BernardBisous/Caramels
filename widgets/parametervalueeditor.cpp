#include "parametervalueeditor.h"
#include "qboxlayout.h"
#include "qlabel.h"

#define PURCENT_INC 5
#define BUTTONS_SIZE 40
ParameterValueEditor::ParameterValueEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr),
      m_mode(modeOffset),m_plot(nullptr),
      m_gain(PURCENT_INC)
{
    setLayout(new QVBoxLayout);

    layout()->addWidget(m_name=new QLabel);
    layout()->addWidget(m_description=new QLabel);
    m_description->setWordWrap(true);

    QWidget* le=new QWidget();
    le->setLayout(new QHBoxLayout);
    le->layout()->setContentsMargins(0,0,0,0);
    le->layout()->addWidget(m_valueEdit=new QLineEdit);
    le->layout()->addWidget(m_units=new QLabel);
    le->layout()->addWidget(m_up=new ToolButton("Up",":/icons/plus"));
    le->layout()->addWidget(m_down=new ToolButton("Down",":/icons/less"));
    layout()->addWidget(le);
    connect(m_valueEdit,SIGNAL(editingFinished()),this,SLOT(valueTyped()));

    layout()->setContentsMargins(0,0,0,0);
  //  layout()->setContentsMargins(0,0,0,0);
    QWidget* modeWidget=new QWidget;
    modeWidget->setLayout(new QHBoxLayout);
    modeWidget->layout()->setContentsMargins(0,0,0,0);

    QWidget* vn=new QWidget;
    vn->setLayout(new QVBoxLayout);
    vn->layout()->setContentsMargins(0,0,0,0);


    modeWidget->layout()->addWidget(m_value=new QLabel);
    modeWidget->layout()->addWidget(addMode(":/icons/gain","Gain"));
    modeWidget->layout()->addWidget(addMode(":/icons/offset","Offset"));
    modeWidget->layout()->addWidget(addMode(":/icons/point","Point"));

    QFont f=font();
    f.setPixelSize(16);
    m_value->setFont(f);
    f.setPixelSize(18);
    m_name->setFont(f);

    layout()->addWidget(modeWidget);


     QWidget*b=new QWidget;
     QHBoxLayout* gr=new QHBoxLayout;
     gr->setContentsMargins(0,0,0,0);

     gr->addWidget(m_left=new ToolButton("Left",":/icons/left"));
     gr->addWidget(m_right=new ToolButton("Right",":/icons/right"));

     gr->setContentsMargins(0,0,0,0);
     b->setLayout(gr);

     layout()->addWidget(b);


     m_up->setRound(BUTTONS_SIZE);
     m_down->setRound(BUTTONS_SIZE);
     m_left->setRound(BUTTONS_SIZE);
     m_right->setRound(BUTTONS_SIZE);

    connect(m_up,SIGNAL(clicked()),this,SLOT(up()));
    connect(m_down,SIGNAL(clicked()),this,SLOT(down()));
    connect(m_left,SIGNAL(clicked()),this,SLOT(left()));
    connect(m_right,SIGNAL(clicked()),this,SLOT(right()));

    setMode(modeOffset);
}

Parameter *ParameterValueEditor::client() const
{
    return m_client;
}

void ParameterValueEditor::setClient(Parameter *newClient)
{

    m_client = newClient;
    refresh();
}

void ParameterValueEditor::refreshPlot()
{
    if(!m_plot)
        return;

    m_plot->refresh();
}

void ParameterValueEditor::refresh()
{


    refreshPlot();
    refreshValue();

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

    //qDebug()<<"move point";
    float val=(inc*m_gain)+100;
    val=val/100;
    if(m_mode!=modeGain)
    {
        float e=qAbs(m_client->maxY()-m_client->minY());
        val=inc*e*m_gain/100;
    }

    move(val);
}

void ParameterValueEditor::move(float val)
{
    if(m_mode!=modeAlone)
        m_client->movePoints(QList<int>(),val,m_mode==modeOffset);

    else
        m_client->movePoints(m_indexes,val,m_mode==modeOffset);

    refresh();


    emit changed();
}

void ParameterValueEditor::edit(QList<int> indexes)
{
    m_indexes=indexes;

    qDebug()<<"editinged"<<indexes;
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




void ParameterValueEditor::refreshValue()
{
    if(!m_client)
        return;

    int n=lastSelected();

    m_name->setText(m_client->name());
    m_description->setText(m_client->description());
    m_units->setText(m_client->units());

    if(m_plot)
        m_plot->refreshTitle();

    QString s;

    qDebug()<<"lastSelected"<<m_client->name()<<n;
    if(n<0)
    {
        float max,min;
        m_client->rangeY(&max,&min);
        m_value->setText(QString::number(min)+"->"+QString::number(max)+m_client->units());
        return;
    }

   bool ok;
   float a=currentValue(&ok);
   if(!ok)
   {
       m_value->clear();
       return;
   }


    m_value->setText(QString::number(a)+m_client->units());
    m_valueEdit->setText(QString::number(a));
}

int ParameterValueEditor::lastSelected()
{
    if(!m_plot || !m_plot->series() || m_plot->series()->selectedPoints().isEmpty())
        return -1;

    return m_plot->series()->selectedPoints().last();
}

float ParameterValueEditor::currentValue(bool *ok)
{
    if(!m_client)
    {
        *ok=false;
        return 0;
    }

    int n=lastSelected();
    if(n<0)
    {
        *ok=false;
        return 0;
    }

    bool sok;
    float a=m_client->valueAtTime(n,&sok);

    if(!ok)
    {
        *ok=false;
        return 0;
    }
    *ok=true;
    return a;

}

void ParameterValueEditor::setGain(float newGain)
{
    m_gain = newGain;
}

void ParameterValueEditor::selectOffset(int inc)
{
    if(!m_plot)
        return;

    QList<int> v,a;
    QLineSeries*sl=m_plot->series();
    a=sl->selectedPoints();

    if(a.isEmpty())
    {
        qDebug()<<"Nou points Selected"<<(inc<0);

        if(inc<0)
            sl->selectPoint(sl->count()-1);
        else
            sl->selectPoint(0);
        return;
    }


    for(int i=0;i<a.count();i++)
    {
        int ia=a[i]+inc;

        if(ia<sl->count() && ia>0)
        {


            v<<ia;
        }
    }
    sl->deselectAllPoints();

    qDebug()<<"Seledd"<<a.count()<<v.count();
    sl->selectPoints(v);

    connect(sl,SIGNAL(selectedPointsChanged()),this,SLOT(seriesSelected()));
    seriesSelected();

}

void ParameterValueEditor::setPlot(ParameterPlot *p)
{
    if(m_plot)
    {
        if(m_plot->series())
        {
            disconnect(m_plot->series(),SIGNAL(selectedPointsChanged()),this,SLOT(seriesSelected()));
            m_plot->series()->setPointsVisible(false);
        }
    }




    m_plot=p;

    if(m_plot)
    {
        if(m_plot->series())
        {
            connect(m_plot->series(),SIGNAL(selectedPointsChanged()),this,SLOT(seriesSelected()));
            m_plot->series()->setPointsVisible(true);
        }
        connect(m_plot->series(),SIGNAL(selectedPointsChanged()),this,SLOT(seriesSelected()));
    }
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
    qDebug()<<"series selected";
    if(m_plot&&m_plot->series())
        edit(m_plot->series()->selectedPoints());
}

void ParameterValueEditor::valueTyped()
{
    bool ok;
    float f=m_valueEdit->text().toFloat(&ok);
    if(ok)
    {
        bool sr;
        float v=currentValue(&sr);

        if(sr)
        {
            move(f-v);
        }
    }

}
