#include "statswidget.h"
#include "Interface/progressbar.h"

StatsWidget::StatsWidget(QWidget *parent)
    : QWidget{parent},m_list()
{
    setLayout(m_form =new QFormLayout);
}

void StatsWidget::setHash(QHash<QString, int> h)
{

    for(int i=0;i<m_list.count();i++)
    {
        delete m_list[i];
    }
    m_list.clear();

    QStringList ls=h.keys();
    for(int i=0;i<ls.count();i++)
    {
        ProgressBar* b=new ProgressBar;
        double f=h.value(ls[i]);
        f=f/100;
        m_form->addRow(ls[i],b);
        b->setValue(f);
        m_list<<b;
    }
}
