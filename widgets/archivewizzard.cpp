#include "archivewizzard.h"
#include "constants.h"




ArchiveWizzard::ArchiveWizzard(Tent *t, QWidget *parent)
    : Wizzard{parent},m_archive(),m_tente(t)
{
    m_deleteAtEnd=true;
    m_welcome=new WizzardSequence(true);
    m_capture=new WizzardCapture(t->cam());
    m_result=new WizzardValue("volume final","","Entrez le volume total après rafinage en gramme","g");
    m_final=new ArchiveOverviewSequence;



    if(t->config())
    {
         m_archive.name=t->config()->name();
         m_archive.start=t->startedDate().date();
         m_archive.end=QDate::currentDate();
    }

    m_final->setArchive(&m_archive);

    m_welcome->loadPicture(":/Images/archive");
    m_welcome->setName("Intro");
    m_welcome->setTexts("Voilà l'étape finale", "Entrons queles données pour archiver ce magnifique travail!\n - Préparez un joli échantillon de produit\n - Préparez la pesée finale");

    addSequence(m_welcome);
    addSequence(m_capture);
    addSequence(m_result);
    addSequence(m_final);

    m_result->loadPicture(":/Images/balance");
    start();
}

void ArchiveWizzard::reactToSequenceFinished(WizzardSequence *s, WizzardResult r)
{
    if(s==m_capture)
    {
        qDebug()<<"captured picture";
        m_archive.pixmap=m_capture->pixmapResult();
    }
    if(s==m_result)
    {
        m_archive.result=m_result->stringValue().toInt();
    }
    if(s==m_final)
    {
        archiveExport();
    }
}

ArchiveWizzard *ArchiveWizzard::archive(Tent *t,QWidget *parent)
{
    return  new ArchiveWizzard(t,parent);
}

void ArchiveWizzard::archiveExport()
{
    QDir dir(ARCHIVE_PATH);

    QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);

    QString p=m_archive.name;
    p.remove(" ");
    QString pn=p;
    int i=1;
    while(subdirs.contains(pn))
    {
        pn=p+QString::number(i);
        i++;
    }
    dir.mkdir(pn);

    dir.cd(pn);
    archiveExport(dir);

}

void ArchiveWizzard::archiveExport(QDir d)
{
    m_tente->exportAll(d.path());
    m_archive.save(d.path());
    emit archived(m_archive);
}

void ArchiveWizzard::finish()
{
   // archiveExport();
    Wizzard::finish();
}



ArchiveOverviewSequence::ArchiveOverviewSequence(QWidget *parent):
    WizzardSequence(parent),m_archive(nullptr),m_preview(nullptr)
{
    m_name="Confirmation";
    addWidget(m_previewWidget=new QWidget);
    m_previewWidget->setLayout(new QHBoxLayout);
}

void ArchiveOverviewSequence::start()
{
    if(m_preview)
        delete m_preview;

    WizzardSequence::start();

    if(!m_archive)
        return;

    m_preview=new ArchiveItem(*m_archive);
    m_previewWidget->layout()->addWidget(m_preview);

    setTexts("Vous avez bien géré", "Félicitations, votre rendement est très bon avec cette "+m_archive->name);
    m_picture->hide();
}

Archive *ArchiveOverviewSequence::archive() const
{
    return m_archive;
}

void ArchiveOverviewSequence::setArchive(Archive *newArchive)
{
    m_archive = newArchive;
}
