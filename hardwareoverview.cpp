#include "hardwareoverview.h"
#include "qboxlayout.h"
#include "qgraphicsitem.h"
#include "qgraphicsscene.h"
#include "qlabel.h"

#define AUTOPOLL false

HardwareOverview::HardwareOverview(QWidget *parent)
    : QWidget(parent),

      m_scene(new QGraphicsScene()),
      m_tent(nullptr),
      m_timer(new QTimer(this)),m_connect(nullptr)
{

    setLayout(new QVBoxLayout);
    layout()->addWidget(m_view=new QGraphicsView);


    m_view->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_view->setBackgroundRole(QPalette::Window);

    m_view->setScene(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);

    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            // Load plant image as background
    QPixmap plantPixmap(":/icons/plant");
    m_scene->addPixmap(plantPixmap.scaledToHeight(400,Qt::SmoothTransformation));
    m_tank=m_scene->addPixmap(QPixmap(":/icons/right").scaledToWidth(50,Qt::SmoothTransformation));
    m_tank->setPos(0,340);
    connect(m_timer, &QTimer::timeout, this, &HardwareOverview::update);





    m_ph=printTextParameters("PH:",QPointF(20,280));
    m_heigh=printTextParameters("Hauteur:",QPointF(10,20));
    m_CO2=printTextParameters("CO2:",QPointF(10,110));

    QRectF rpump(18,320,100,20);

    m_pumps=new QGraphicsRectItem(rpump);
    m_pumps->setBrush(palette().highlight());
    m_scene->addItem(m_pumps);

   // m_pumps=m_scene->addRect(rpump,QPen(),palette().highlight());

    m_sun=m_scene->addPixmap(QPixmap(":/icons/sun").scaled(60,60,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    m_sun->setPos(QPointF(10,-80));
    m_spectrum=printTextParameters("Color:",QPointF(70,-60));

    QWidget*sp=new QWidget;
    sp->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout()->addWidget(sp);


    if(AUTOPOLL)
        m_timer->start(1000); // Update every second



}

QGraphicsTextItem * HardwareOverview::printTextParameters(const QString &text, QPointF position)
{
    QFont f=font();

    QGraphicsTextItem *textItem = new QGraphicsTextItem(text);
    textItem->setPos(position);
    auto a=m_scene->addText(text);
    a->setParentItem(textItem);
    a->setPos(QPointF(0,-15));


    f.setPointSize(14);
    textItem->setFont(f);
    m_scene->addItem(textItem);
    return textItem;
}

void HardwareOverview::handle(Tent *t)
{
    m_tent=t;

    if(!AUTOPOLL)
        connect(t,SIGNAL(sensorsAquiered()),this,SLOT(update()));

    connect(t,SIGNAL(connectedHardware(bool)),this,SLOT(connectedChanged(bool)));


    auto l=t->injectors();
    for(int i=0;i<l.count();i++)
        addInjector(l[i]);

    QWidget*sp=new QWidget;

    layout()->addWidget(sp);

    if(t&&t->temperatures())
        addDhts(t->temperatures()->dht());

    showConnect(t->connected());
    update();
}

void HardwareOverview::showConnect(bool s)
{



    if(s && !m_connect)
        return;

    if(!s && !m_connect)
    {
        m_connect=new TopConnect(m_tent,this);
        return;
    }

    if(!s)
           m_connect->start();
    else
        m_connect->stop();
}

void HardwareOverview::addInjector(ChemicalInjector *t)
{
    ChemicalInjectorWidget* tw=new ChemicalInjectorWidget(t,this);
    m_injectors.append(tw);
    layout()->addWidget(tw);
}

void HardwareOverview::addDhts(QList<DHT> l)
{
    for(int i=0;i<l.count();i++)
    {

        QGraphicsTextItem *textItemt = new QGraphicsTextItem("temp");
        textItemt->setPos(100,i*100);
        QGraphicsTextItem *textItemh = new QGraphicsTextItem("hum");
        textItemh->setPos(100,i*100+20);


        m_scene->addItem(textItemh);
        m_scene->addItem(textItemt);
        m_dhtItems<<textItemh<<textItemt;

    }
    m_dhts<<l;
}

void HardwareOverview::updateDHTs()
{
    for(int i=0;i<m_dhts.count();i++)
    {
        m_dhtItems[i*2]->setPlainText(m_dhts[i].temp->userValue());
        m_dhtItems[i*2+1]->setPlainText(m_dhts[i].humidity->userValue());
    }
}

void HardwareOverview::update()
{

    if(!m_tent)
        return;


    m_ph->setPlainText(QString::number(m_tent->PH(),'f',1));
    m_CO2->setPlainText(QString::number(m_tent->CO2(),'f',1)+"ppm");
    m_heigh->setPlainText(QString::number(m_tent->height(),'f',2)+"m");


    if(m_tent->temperatures())
        updateDHTs();

    if(m_pumps)
    {

        bool sp=false;
        if(m_tent->pumps())
            sp=m_tent->pumps()->isDry();

        if( sp)
            m_pumps->hide();
        else
            m_pumps->show();

        m_pumps->update();

        sp=false;
        if(m_tent->pumps())
            sp=m_tent->pumps()->filling();
        if(sp)
            m_tank->show();
        else
            m_tank->hide();
    }


    if(m_tent->lightPower()==0)
    {
        m_sun->hide();
        m_spectrum->hide();
    }

    else {

        m_sun->show();
        m_spectrum->show();

        m_spectrum->setPlainText(QString::number(m_tent->lightSpectrum(),'f',1)+"nm");
    }


    for(int i=0;i<m_injectors.count();i++)
    {
        m_injectors[i]->refresh();
    }

}

void HardwareOverview::connectedChanged(bool s)
{
    showConnect(s);
}

TopConnect::TopConnect(Tent *t, QWidget *parent):TopWidget(parent)
{
    m_central->setLayout(new QVBoxLayout);
    m_central->layout()->addWidget(new QLabel("Electronique introuvable"));
    m_central->layout()->addWidget(m_editor=new SerialEditor);

   // connect(t,SIGNAL(connectedHardware(bool)),this,SLOT(trigSlot(bool)));
    m_editor->handle(t);
    start();
}




ChemicalInjectorWidget::ChemicalInjectorWidget(ChemicalInjector* i,QWidget *parent):
    QWidget(parent),m_client(i)
{
    setLayout(new QHBoxLayout);
    layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(m_name=new QLabel);
    layout()->addWidget(m_bar=new ProgressBar);
    refresh();

    m_bar->setMinimumHeight(15);
}

void ChemicalInjectorWidget::refresh()
{
    if(!m_client->isEnabled())
    {
        setHidden(true);
        return;
    }


    if(m_client->isLow())
        m_bar->setBrush(QBrush(Qt::red));

    else if (m_client->state()!=ChemicalInjector::ready)
        m_bar->setBrush(QBrush(Qt::white));

    else
        m_bar->setBrush(QBrush());

    m_name->setText(m_client->name());
    m_bar->setValue(m_client->remainingPurcent());
}
