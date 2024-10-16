#include "archivewidget.h"
#include "config/archive.h"
#include "config/growconfig.h"
#include "qboxlayout.h"
#include "qdir.h"

ArchiveWidget::ArchiveWidget(QWidget *parent)
    : QWidget{parent}
{
    setLayout(new QVBoxLayout);

    QLabel* l=new QLabel("Précédentes récoltes:");

    QFont f=font();
    f.setPointSize(28);
    l->setFont(f);

    QWidget* s=new QWidget;
    s->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout()->addWidget(s);

    layout()->addWidget(l);

    layout()->addWidget(m_list=new ScrollArea);
    s=new QWidget;
    s->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout()->addWidget(s);
    m_list->setHorizontal();
    load();
}

void ArchiveWidget::load()
{
    QDir dir("archives");


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
    ArchiveItem* we=new ArchiveItem(c);
    m_list->addActionWidget(we);
}

ArchiveItem::ArchiveItem(Archive &c, QWidget *parent):ActionWidget(parent)
{

    setFixedWidth(400);
    setLayout(new QVBoxLayout);
    layout()->setContentsMargins(20,20,20,20);
    layout()->addWidget(m_pix=new QLabel);
    layout()->addWidget(m_name=new QLabel(c.name));

    if(!c.description.isEmpty())
        layout()->addWidget(m_desc=new QLabel(c.description));

    QWidget* fw=new QWidget;
    fw->setLayout(m_form=new QFormLayout);

    m_form->addRow("Date",m_date=new QLabel(c.start.toString("dd.MM.yy")+" - "+c.end.toString("dd.MM.yy")));
    m_form->addRow("Volume",m_result=new QLabel(QString::number(c.result)+"g"));
    m_form->addRow("Rendement",m_date=new QLabel(QString::number(c.result/c.plants)+"g/plants"));

    layout()->addWidget(fw);

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



    setHoverable(false);
    setIgnoreClick(true);

    QFont f=font();
    f.setPointSize(18);
    m_name->setFont(f);
    m_pix->setPixmap(c.pixmap.scaled(400,400,Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

void ArchiveItem::reuseSlot()
{
    emit reuse();

}
