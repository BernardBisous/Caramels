#include "drawerselector.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QBrush>


DrawerSelector::DrawerSelector(QWidget *parent)
    : QWidget{parent},m_list()
{
    setLayout(new QHBoxLayout);
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    layout()->setSpacing(30);

    layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(m_actionWidget=new QWidget);
    m_actionWidget->setLayout(new QHBoxLayout);
    m_actionWidget->setContentsMargins(0,0,0,0);
    layout()->addWidget(m_help=new ToolButton("Help",":/icons/help"));
    m_help->setRound(40);
    connect(m_help,SIGNAL(clicked()),this,SLOT(helpSlot()));
    m_help->setCheckable(true);

}

void DrawerSelector::setActions(QStringList s)
{
   // qDebug()<<"Drawer selector refresh"<<s;
    clear();
    for(int i=0;i<s.count();i++)
    {
        addAction(s[i]);
    }
}

void DrawerSelector::addAction(QString a)
{
    QPushButton* b=new DrawerSelectorItem(a);
    connect(b,SIGNAL(clicked()),this, SLOT(clickedSlot()));
    m_actionWidget->layout()->addWidget(b);
    m_list.append(a);

    setActive(m_list.count()-1);
}

void DrawerSelector::setActive(int index)
{
    int i=0;
    while (m_actionWidget->layout()->itemAt(i))
    {
        QPushButton*b=dynamic_cast<QPushButton*>(m_actionWidget->layout()->itemAt(i)->widget());
         if(b)
         {
             b->setChecked(i==index);
             emit changed(i);
         }
         i++;

    }
}

QString DrawerSelector::currentAction(int* id)
{
    int i=0;
    while (m_actionWidget->layout()->itemAt(i))
    {
         QPushButton*b=dynamic_cast<QPushButton*>(m_actionWidget->layout()->itemAt(i)->widget());
         if(b)
         {
             if(b->isChecked())
             {
                 if(id)
                     *id=i;
                 return b->text();
             }
         }
         i++;
    }

    return QString();
}
void DrawerSelector::hide(QStringList s, bool sb)
{
   auto a=list();
   for(int j=0;j<s.count();j++)
   for(int i=0;i<a.count();i++)
   {
       if(s[j]==a[i]->text())
           a[i]->setHidden(sb);
   }
}

void DrawerSelector::hideAll(bool s)
{
    auto a=list();
    for(int i=0;i<a.count();i++)
    {
        a[i]->setHidden(s);
    }
}

QList<DrawerSelectorItem *> DrawerSelector::list()
{
    QList<DrawerSelectorItem *>out;
    for(int i=0;i<m_actionWidget->layout()->count();i++)
    {
        auto a=dynamic_cast<DrawerSelectorItem*>(m_actionWidget->layout()->itemAt(i)->widget());
        if(a)
            out.append(a);
    }
    return out;

}

bool DrawerSelector::isHidden(int id)
{

    auto l=list();

    if(id<l.count() && id>=0)
        return l[id]->isHidden();

    return true;
}

int DrawerSelector::currentIndex()
{
    int out=0;
    currentAction(&out);
    return out;
}

bool DrawerSelector::nextIsHidden()
{
    return isHidden(currentIndex()+1);
}

void DrawerSelector::setHelpHidden(bool s)
{
    m_help->setHidden(s);
}

void DrawerSelector::disable(int i, bool s)
{
    auto l=list();
    if(l.count()>i)
        l[i]->setDisabled(s);
}


void DrawerSelector::clickedSlot()
{
     QPushButton*b=dynamic_cast<QPushButton*>(sender());
     if(b && b->isEnabled())
     {

         int i=m_actionWidget->layout()->indexOf(b);
         emit triggered(i);
         setActive(i);
     }
}

void DrawerSelector::helpSlot()
{
    emit help();
}

void DrawerSelector::clear()
{
    if(!layout())
        return;

    m_list.clear();
    QLayoutItem*t=nullptr;
    do
    {
        t=m_actionWidget->layout()->takeAt(0);
        if(t)
        {
             QPushButton*b=dynamic_cast<QPushButton*>(t->widget());
             if(b)
                 delete b;
        }

    }while(t);
}



DrawerSelectorItem::DrawerSelectorItem(QString t,QWidget *parent):
    QPushButton(t,parent)
{
    setCheckable(true);
    setForegroundRole(QPalette::Base);

   // setFixedHeight(50);

    QFont f=font();
    f.setPixelSize(14);
    setFont(f);

    m_hovered=false;
}

void DrawerSelectorItem::paintEvent(QPaintEvent *event)
{

    int w=1;

    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.setBrush(palette().text());
    painter.setPen(palette().text().color());

    QRect te;
    painter.drawText(rect().translated(0,-2*w),Qt::AlignCenter|Qt::AlignTop,text(),&te);

    if (isEnabled() && (isChecked() || m_hovered))
    {
        painter.setRenderHint(QPainter::Antialiasing);
        QRect r=te.marginsAdded(QMargins(w,w,w,w));
        r.setTop(r.bottom()-w*2);
        r.translate(0,r.height());

        QBrush b=palette().highlight();
        QPen p=b.color().lighter();

        if(m_hovered && !isChecked())
           painter.setOpacity(0.5);

        painter.setBrush(b);
        painter.setPen(p);
        painter.drawRoundedRect(r,w,w);
    }

}
void DrawerSelectorItem::enterEvent(QEnterEvent *)
{
    m_hovered=true;
}

void DrawerSelectorItem::leaveEvent(QEvent *)
{
    m_hovered=false;
}
