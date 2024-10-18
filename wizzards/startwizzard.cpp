#include "startwizzard.h"
#include "qapplication.h"


StartWizzard::StartWizzard(Tent* t,QWidget *parent)
    : Wizzard{parent}
{
    setName("Nouvelle plantation");
    m_source=new ConfigWizzardSequence(t);
  //  m_editor=new ConfigEditorSequence(t);
    addSequence(m_source);
    //addSequence(m_editor);
    m_topSpacer->hide();
    m_bottomSpacer->hide();
}

StartWizzard *StartWizzard::execute(Tent *t)
{
    StartWizzard* out=new StartWizzard(t,QApplication::activeWindow());
    out->start();
    return out;
}

ConfigWizzardSequence::ConfigWizzardSequence(Tent *t, QWidget *parent)
    :WizzardSequence(true,parent),m_tent(t)
{
    setTexts("Sélectionner le programme de votre choix","Chaque programme détermine les différents paramètres de culture, tout est automatique.");
    setName("Configuration");


    loadPicture("");
    m_picture->setHidden(true);

    addWidget(m_archives=new ArchiveWidget(true));
    connect(m_archives, SIGNAL(reuse(Archive)),this,SLOT(reuseSlot(Archive)));
}

void ConfigWizzardSequence::reuseSlot(Archive a)
{
    m_tent->startArchive(a);
    finish();
}



ConfigEditorSequence::ConfigEditorSequence(Tent *t, QWidget *parent):
    WizzardSequence(false,parent),m_tent(t)
{
    setName("Vue d'ensemble");
    setTexts("Voici le plan",
                    "Parcourez les différents paramètres et modifiez les si besoin. Il est recommandé de passer cette étape");

    loadPicture("");
    m_picture->setHidden(true);
   // addWidget(m_config=new ConfigEditor);
}

void ConfigEditorSequence::start()
{

    m_config->handle(m_tent->config());
    WizzardSequence::start();
}
