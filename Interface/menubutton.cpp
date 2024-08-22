#include "menubutton.h"
#include "qevent.h"
#include "qpainter.h"

constexpr auto MENU_SEPARATOR = "/";
#define ARROW_SIZE 3
MenuButton::MenuButton(QString s,QWidget *parent) :
    QPushButton (parent),
    m_activeAction(nullptr),
    m_defaultText(s),
    m_printActive(false),
    m_activeAtTop(true),
    m_uniqueActive(true)

{
    QMenu* menu=new QMenu(this);
    setMenu(menu);
    setText(s);

    if(s.isEmpty())
        setPrintActive(true);
}

void MenuButton::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();

    if(!isEnabled()) //TODO do something when disabled
    {
        painter.restore();
        painter.end();
        QWidget::paintEvent(e);
        return;
    }





    QBrush b1;
    if(m_hover)
        b1= palette().button();

    else
        b1= palette().window();



    painter.setBrush(b1);


    QColor c=b1.color();


    QPen p;
    p.setColor(palette().highlight().color());
    /*
    if(m_mode==noBorder || (m_appear && !m_hover))
        p.setColor(c);

    else if(m_mode==highlighted)
        p.setColor(palette().text().color());

    else if(m_mode==customPen)
        p=m_borderPen;

    else if(m_mode==appearingBorder)
    {
        if(m_hover)
            p.setColor(palette().highlight().color());
        else
            p.setColor(c);
    }

*/



    painter.setPen(p);


    float rx=e->rect().height()/2;
    // float ry=rx*width()/height();
    float ry=rx;
    painter.drawRoundedRect(e->rect().marginsRemoved(QMargins(p.width(),p.width(),p.width(),p.width())),rx,ry);

    painter.setBrush(palette().text());
    QPen pe(palette().text().color());
    pe.setWidth(2);
    painter.setPen(pe);

    int l=ARROW_SIZE;

    QRect tr=e->rect();
    tr.setLeft(e->rect().right()-5*l);

    QPoint pc=tr.center()+QPoint(0,l);
    QPoint pa=pc+QPoint(-l,-l);
    QPoint pb=pc+QPoint(l,-l);

    /*
    tr.setBottom(w);
    tr=tr.marginsRemoved(QMargins(3,3,3,3));
    painter.drawEllipse(tr);
    tr.translate(0,w);
    painter.drawEllipse(tr);
    tr.translate(0,w);
    painter.drawEllipse(tr);
    painter.drawText(e->rect(),text(),QTextOption(Qt::AlignCenter | Qt::AlignHCenter));
    */

    painter.drawText(e->rect().translated(QPoint(-l*2,0)),text(),QTextOption(Qt::AlignCenter | Qt::AlignHCenter));
    painter.drawLine(pc,pa);
    painter.drawLine(pc,pb);
    painter.restore();
    painter.end();

    update();

    QWidget::paintEvent(e);
}

void MenuButton::nouvAction(QString iconUrl, QString text, QMenu *sub)
{
    QAction*a=nouvAction(text,sub);
    a->setIcon(QIcon(iconUrl));
}



void MenuButton::addActions(QStringList l, QMenu *m)
{
    for (int i=0;i<l.count();i++) {
        nouvAction(l[i],m);
    }
}
QAction* MenuButton::nouvAction(QString nam,QMenu* sub)
{
    QAction* act=new QAction(nam,this);
    connect(act,SIGNAL(triggered()),this,SLOT(actionHandle()));

    QString s=menuPath(sub);

    if(!s.isEmpty())
        s+=MENU_SEPARATOR;

    act->setWhatsThis(s+nam);
    //qDebug()<<"addAction"<<act->whatsThis();

    if(sub)
        sub->addAction(act);

    else
        menu()->addAction(act);

    return act;
}



QString MenuButton::actionAbsolutePath(QAction *a)
{
    if(!a)
        return "";

    return a->whatsThis();
}

void MenuButton::actionHandle()
{

    QAction* act=qobject_cast<QAction*>(sender());
    setActivate(act);
    triggAction(act);


}

bool MenuButton::printActive() const
{
    return m_printActive;
}

void MenuButton::setPrintActive(bool printActive)
{
    m_printActive = printActive;
}

void MenuButton::enterEvent(QEnterEvent *event)
{
    m_hover=true;
}

void MenuButton::leaveEvent(QEvent *event)
{
    m_hover=false;
}

void MenuButton::mousePressEvent(QMouseEvent *e)
{
    m_press=true;
    QPushButton::mousePressEvent(e);
}

void MenuButton::mouseReleaseEvent(QMouseEvent *e)
{
    m_press=false;
    QPushButton::mouseReleaseEvent(e);
}
void MenuButton::clear()
{
    menu()->clear();
}

QString MenuButton::textValue(bool absolute)
{
    if(!m_activeAction)
        return "";
    
    QString s=m_activeAction->whatsThis();

    if(!absolute)
    {
        QStringList l=s.split(MENU_SEPARATOR);
        return l.last();
    }

    return s;
}

QString MenuButton::menuPath(QMenu *a)
{
    if(!a)
        return "";

    if(a==menu())
        return "";

    QString se=findMenuIn(menu(),a);
    return se;
}

void MenuButton::setDefaultText(const QString &defaultText)
{
    m_defaultText = defaultText;
}

void MenuButton::setTitle(QString s)
{
    QFontMetrics f(font());
    setMinimumWidth(f.boundingRect(s).width()+ARROW_SIZE*5);

    menu()->setTitle(s);
}




QString MenuButton::findMenuIn(QMenu *m, QMenu *toFind)
{
    if(!m||!toFind)
        return "";

    QList<QAction*> l=m->actions();

    for(int i=0;i<l.count();i++)
    {
        if(toFind==l[i]->menu())
        {
            return MENU_SEPARATOR+l[i]->whatsThis();
        }

        QString s= findMenuIn(l[i]->menu(),toFind);
        if(!s.isEmpty())
            return s;
    }
    return "";
}



void MenuButton::triggAction(QAction *act)
{
    emit actionTrigg(act);
    emit actionTrigg(textValue());
    emit actionTrigg();
}

void MenuButton::refresh()
{

    if(!m_printActive)
        return;

    if(!m_activeAction)
    {
        setText(m_defaultText);
        return;
    }

    setText(m_activeAction->text());
    menu()->setTitle(m_activeAction->text());

}


void MenuButton::setActivate(QString act)
{
    for(int i=0;i<menu()->actions().count();i++)
    {
        if(menu()->actions()[i]->text()==act)
        {
            setActivate(menu()->actions().at(i));
        }
        else menu()->actions()[i]->setIcon(QIcon());
    }
}

void MenuButton::setActivate(QAction *act)
{
    if(m_uniqueActive)
    clearActive(menu());

    m_activeAction=act;

    if(m_activeAction)
    {
        act->setIcon(QIcon(":/icons/next"));
        act->setIconVisibleInMenu(true);

    }
    if(m_printActive)
        menu()->setTitle(act->text());


    refresh();
    return;
}

void MenuButton::setUniqueActiveAction(bool s)
{
    m_uniqueActive=s;
}

QStringList MenuButton::actionsList()
{
    // Do the same for sub menus ??

    return actionsList(menu());
}

QStringList MenuButton::actionsList(QMenu *m)
{
    QList<QAction*> l=m->actions();
    QStringList out;
    for(int i=0;i<l.count();i++)
    {
        if(l[i]->menu())
        {
            out+=actionsList(l[i]->menu());
        }
        else
            out+=l[i]->text();
    }
    return out;
}

QMenu *MenuButton::subMenu(QString path, QMenu *m_masterMenu, QString iconUrl)
{
    QStringList s=path.split(MENU_SEPARATOR);

    QMenu* m;

    if(!m_masterMenu)
        m=menu();
    else
        m=m_masterMenu;

    for(int i=0;i<s.count() && m;i++)
    {
        //qDebug()<<"In menu"<<s[i];
        QMenu* m2=nullptr;
        bool t=false;
        QList<QAction*> l=m->actions();
        for(int j=0;j<l.count() && !t;j++)
        {
            if(s[i]==l[j]->text())
            {
                t=true;
                m2=l[j]->menu();
            }
        }

        if(!m2)
        {
            //qDebug()<<"button create menu"<<s[i]<<"in "<<m->title();
            QAction* a=nouvAction(s[i],m);

            if(!iconUrl.isEmpty())
                a->setIcon(QIcon(iconUrl));

            m2=new QMenu(this);
            m2->setTitle(a->text());
            a->setMenu(m2);
        }

        m=m2;
    }

    return m;

}

void MenuButton::disableAction(QString s)
{
    auto l =menu()->actions();
    for(int i=0;i<l.size();i++)
        l[i]->setDisabled(s==l[i]->text());
}

void MenuButton::clearActive(QMenu *m)
{
    if(!m)
        return;
    QList<QAction*> l=m->actions();
    for(int i=0;i<l.count();i++)
    {
        clearActive(l[i]->menu());
        l[i]->setIcon(QIcon());
        l[i]->setIconVisibleInMenu(false);
    }
}
