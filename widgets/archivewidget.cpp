#include "archivewidget.h"
#include "config/archive.h"
#include "constants.h"
#include "qboxlayout.h"
#include "qdir.h"
#include<QDesktopServices>
ArchiveWidget::ArchiveWidget(bool abstract,QWidget *parent)
    : QWidget{parent},m_personnal(false),m_astracted(abstract)
{
    setLayout(new QVBoxLayout);

    if(!abstract)
    {
        QLabel* l=new QLabel("Précédentes récoltes:");

        QFont f=font();
        f.setPointSize(28);
        l->setFont(f);

        QWidget* s=new QWidget;
        s->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        layout()->addWidget(s);
        layout()->addWidget(l);
    }

    layout()->addWidget(m_source=new SwitchCheckBox("Réutiliser mes archives"));
    m_source->setChecked(m_personnal);

    connect(m_source,SIGNAL(stateChanged(int)),this,SLOT(sourceSlot()));

    layout()->addWidget(m_list=new ScrollArea);
    m_list->addSpacer();
    load();
}

void ArchiveWidget::load()
{

    m_list->clear();

    QString path=CONFIGS_PATH;
    if(m_personnal)
        path=ARCHIVE_PATH;

    QDir dir(path);

    QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);

    for(int i=0;i<subdirs.count();i++)
    {
        QDir de=dir;
        de.cd(subdirs[i]);
        loadArchive(de.absolutePath());
    }

}

void ArchiveWidget::loadArchive(QString s)
{
    Archive c;
    c.load(s);

    ArchiveItem* we=new ArchiveItem(c,!m_astracted);


    if(!m_astracted)
        connect(we,SIGNAL(reuse()),this,SLOT(reuseSlot()));
    else
        connect(we,SIGNAL(clicked()),this,SLOT(reuseSlot()));

    m_list->addActionWidget(we);

}

void ArchiveWidget::usePersonnal(bool s)
{
    m_personnal=s;
    load();
}

void ArchiveWidget::reuseSlot()
{
    ArchiveItem* i=dynamic_cast<ArchiveItem* >(sender());
    if(i)
    {
        Archive a=i->archive();
        emit reuse(a);
    }
}

void ArchiveWidget::sourceSlot()
{
    bool b=m_source->isChecked();
    usePersonnal(b);
}

void ArchiveWidget::setAstracted(bool newAstracted)
{
    m_astracted = newAstracted;
}

ArchiveItem::ArchiveItem(Archive c,bool but, QWidget *parent):ActionWidget(parent),m_archive(c)
{

    connect(this,SIGNAL(hovered(bool)),this,SLOT(hoverSlot(bool)));
    setFixedWidth(420);

    setLayout(new QHBoxLayout);
    layout()->setSpacing(20);
    layout()->setContentsMargins(20,20,20,20);
    if(!c.pixmap.isNull())
    {
        layout()->addWidget(m_pix=new QLabel);
        m_pix->setPixmap(c.pixmap.scaled(200,200,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
    }


    QWidget*dw=new QWidget;
    QVBoxLayout* hb=new QVBoxLayout;
    dw->setLayout(hb);
    layout()->addWidget(dw);
    dw->layout()->setContentsMargins(0,0,0,0);

    dw->layout()->addWidget(m_name=new QLabel(c.name));
    QFont f=font();
    f.setPointSize(18);
    m_name->setFont(f);

    if(!c.description.isEmpty())
        dw->layout()->addWidget(m_desc=new QLabel(c.description));


    if(!c.stats.isEmpty())
    {
        dw->layout()->addWidget(m_stats=new StatsWidget);
        m_stats->layout()->setContentsMargins(0,0,0,0);
        m_stats->setHash(c.stats);
    }

    QWidget* fw=new QWidget;
    fw->setLayout(m_form=new QFormLayout);

    m_form->addRow("Date",m_date=new QLabel(c.start.toString("dd.MM.yy")+" - "+c.end.toString("dd.MM.yy")));
    m_form->addRow("Volume",m_result=new QLabel(QString::number(c.result)+"g"));
    m_form->setContentsMargins(0,0,0,0);
    if(c.plants)// generates crashes !
        m_form->addRow("Rendement",m_plants=new QLabel(QString::number(c.result/c.plants)+"g/plants"));

    dw->layout()->addWidget(fw);
    hb->addStretch();

    QStringList ls=c.meta.keys();
    for(int i=0;i<ls.count();i++)
    {
       m_form->addRow(ls[i],new QLabel(c.meta.value(ls[i])));
    }


    if(!c.link.isEmpty() || !c.supplier.isEmpty())
    {
        m_form->addRow("Supplier",m_supply=new QLabel);
        if(!c.link.isEmpty())
        {

            m_supply->setTextFormat(Qt::RichText);
            m_supply->setOpenExternalLinks(true);
            if(c.supplier.isEmpty())
                c.supplier="Link";

            m_supply->setText("<a href='"+c.link+"'>"+c.supplier+"</a>");
        }

        else
            m_supply->setText(c.supplier);
    }



    m_buttons=nullptr;
    if(but)
    {
        setIgnoreClick(true);


        m_buttons=new QWidget;
        m_buttons->setLayout(new QVBoxLayout);

        QWidget*wsb=new QWidget;
        wsb->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
        m_buttons->layout()->addWidget(wsb);
        m_buttons->layout()->setContentsMargins(0,0,0,0);

        if(c.hasConfig())
        {
            m_buttons->layout()->addWidget(m_reuseButton=new ToolButton("Reuse",":/icons/left"));
            connect(m_reuseButton,SIGNAL(clicked()),this,SLOT(reuseSlot()));

            m_reuseButton->setRound();
        }
        if(!c.path.isEmpty())
        {
            m_buttons->layout()->addWidget(m_openButton=new ToolButton("Open",":/icons/logs"));
            m_openButton->setRound();
            connect(m_openButton,SIGNAL(clicked()),this,SLOT(openSlot()));
        }
        m_buttons->hide();

        QWidget*wss=new QWidget;
        wss->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
        layout()->addWidget(wss);
        layout()->addWidget(m_buttons);
    }




}



void ArchiveItem::reuseSlot()
{
    emit reuse();

}

void ArchiveItem::hoverSlot(bool s)
{
    if(m_buttons)
    m_buttons->setHidden(!s);
}

void ArchiveItem::openSlot()
{
    QDesktopServices::openUrl(m_archive.path);
}

Archive ArchiveItem::archive()
{
    return m_archive;
}
