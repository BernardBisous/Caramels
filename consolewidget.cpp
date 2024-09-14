#include "consolewidget.h"
#include "qboxlayout.h"

ConsoleWidget::ConsoleWidget(QWidget *parent) : QWidget{parent}
{
    setLayout(new QVBoxLayout);
    layout()->addWidget(m_console=new QTextEdit);
    m_console->setWordWrapMode(QTextOption::WrapAnywhere);
    m_console->setBackgroundRole(QPalette::Window);

    QFont f=font();
    f.setPointSize(8);
    m_console->setFont(f);
}

void ConsoleWidget::setTent(Tent *t)
{
    if(!t)
        return;


    m_console->setPlainText(t->allConsole());
    connect(t,SIGNAL(dateChanged(QDateTime)),this,SLOT(clear()));
    connect(t,SIGNAL(consoleRequest(QString)),this,SLOT(append(QString)));
}

void ConsoleWidget::append(QString s)
{
    m_console->append(s);
}

void ConsoleWidget::clear()
{
    m_console->clear();
}
