#include "consolewidget.h"
#include "qboxlayout.h"

ConsoleWidget::ConsoleWidget(QWidget *parent) : QWidget{parent},m_tent(nullptr),m_enable(true)
{
    setLayout(new QVBoxLayout);
    layout()->addWidget(m_scroll=new ScrollArea);
    m_text=new QLabel;
    m_scroll->setWidget(m_text);
    m_text->setWordWrap(true);
    m_scroll->setBackgroundRole(QPalette::Window);
    m_text->setAlignment(Qt::AlignTop);

    QFont f=font();
    f.setPointSize(8);
    m_text->setFont(f);

    setFixedWidth(300);
}

void ConsoleWidget::setTent(Tent *t)
{
    if(!t)
        return;


    m_tent=t;

    if(!t)
        return;

    reset();

    connect(t,SIGNAL(dateChanged(QDateTime)),this,SLOT(reset()));
    connect(t,SIGNAL(consoleRequest(QString)),this,SLOT(append(QString)));
}

void ConsoleWidget::setEnableConsole(bool newEnable)
{
    m_enable = newEnable;
    setHidden(!newEnable);

    if(newEnable)
        reset();

    else clear();
}

void ConsoleWidget::reset()
{
    if(!m_tent)
    {
        m_text->clear();
        return;
    }

    m_text->setText(m_tent->allConsole());
    m_scroll->setWidget(m_text);
    m_scroll->scrollDown();
}

bool ConsoleWidget::enabledConsole() const
{
    return m_enable;
}



void ConsoleWidget::append(QString s)
{
    if(!m_enable)
        return;

    m_text->setText(m_text->text()+s);
    m_scroll->setWidget(m_text);
    m_scroll->scrollDown();
}

void ConsoleWidget::clear()
{
    m_text->clear();
}
