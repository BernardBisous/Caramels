#include "statewidget.h"
#include "qboxlayout.h"

StateWidget::StateWidget(QWidget *parent)
    : QWidget{parent},m_current(nullptr)
{
    setLayout(new QVBoxLayout);
    layout()->addWidget(new QLabel("Erreur détectée:"));
    layout()->addWidget(m_editor=new ActionWidget);

    layout()->setContentsMargins(0,0,0,0);
    m_editor->setLayout(new QHBoxLayout);
    m_editor->layout()->addWidget(m_icon=new QLabel);
    m_icon->setFixedSize(60,60);
    m_icon->setPixmap(QPixmap(":/icons/danger").scaled(QSize(60,60),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    QWidget* ws=new QWidget;
    ws->setLayout(new QVBoxLayout);
    ws->setContentsMargins(0,0,0,0);
    ws->layout()->addWidget(m_nameLabel=new QLabel);
    ws->layout()->addWidget(m_diagnose=new QLabel);
    m_editor->layout()->addWidget(ws);

    QFont f=font();
    f.setPointSize(f.pointSize()+4);
    m_nameLabel->setFont(f);
    m_diagnose->setWordWrap(true);

    m_editor->setHoverable(false);
    m_editor->setIgnoreClick(true);


    QPen p;
    p.setWidth(3);
    p.setColor(Qt::red);
    p.setStyle(Qt::DotLine);
    m_editor->setBorderPen(p);
}

void StateWidget::handle(StateNotifier *s)
{
    m_client=s;
    if(s)
        connect(s,SIGNAL(changed()),this,SLOT(errorSlot()));

    refresh();
}

void StateWidget::setCurrent(DeviceState *s)
{
    if(!s)
    {
        return;
    }

    if(s->device())
        m_nameLabel->setText(s->device()->name());
    else if(s->unit())
        m_nameLabel->setText(s->unit()->name());

    m_diagnose->setText(s->diagnosis());
    m_current=s;

}

void StateWidget::refresh()
{
    auto l=m_client->bads();

    setHidden(l.isEmpty());

    if(l.count())
        setCurrent(l.first());
    else
        setCurrent(nullptr);
}

void StateWidget::triggSlot()
{
    emit trig(m_current);
}

void StateWidget::errorSlot()
{
    refresh();
}
