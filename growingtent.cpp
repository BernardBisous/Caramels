#include "growingtent.h"
#include "widgets/archivewidget.h"
#include "widgets/setupwizzard.h"
#include "widgets/archivewizzard.h"

#define HELP_FILE "help.pdf"
#include <QDesktopServices>
#include <QApplication>
#include <QStyleFactory>
enum {General=0,Hardware,Archive=50};
GrowingTent::GrowingTent(QWidget* parent)
    : QMainWindow(parent)

{
    loadStyle();

    m_currentConfig=new GrowConfig;
    m_tent=new Tent;
    m_tent->setConfig(m_currentConfig);
    m_tent->begin();

    QWidget* bar=new QWidget;
    bar->setAutoFillBackground(true);
    bar->setBackgroundRole(QPalette::Base);
    bar->setLayout(new QHBoxLayout);
    QLabel* pix;
    bar->layout()->addWidget(pix=new QLabel);
    pix->setPixmap(QPixmap(":/icons/logo").scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    QWidget* spd=new QWidget;
    spd->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    bar->layout()->addWidget(spd);

    bar->layout()->addWidget(m_selector=new DrawerSelector);

    QWidget* sps=new QWidget;
    sps->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    bar->layout()->addWidget(sps);
    bar->layout()->addWidget(m_settings=new ToolButton("Settings",":/icons/settings"));
    bar->layout()->addWidget(m_help=new ToolButton("Console",":/icons/logs"));
    bar->layout()->addWidget(m_quit=new ToolButton("Quit",":/icons/delete"));
    bar->layout()->setSpacing(20);
    m_settings->setRound(40);
    m_help->setRound(40);
    m_quit->setRound(50);
    connect(m_help,SIGNAL(clicked()),this,SLOT(help()));
    connect(m_quit,SIGNAL(clicked()),this,SLOT(quitSlot()));
    connect(m_settings,SIGNAL(clicked()),this,SLOT(settingsSlot()));


    QWidget* c=new QWidget;
    c->setLayout(new QVBoxLayout);
    c->layout()->addWidget(bar);
    c->layout()->setContentsMargins(0,0,0,0);

    QWidget* mc=new QWidget;
    mc->setLayout(new QHBoxLayout);
    mc->layout()->addWidget(m_tentOverview=new HardwareOverview);
    m_tentOverview->handle(m_tent);
    m_tentOverview->setFixedWidth(200);
    mc->layout()->addWidget(m_stack=new QStackedWidget);

    m_settingsWidget=new QWidget;
    m_settingsWidget->setLayout(new QVBoxLayout);
    m_settingsWidget->layout()->addWidget(m_devices=new DeviceListWidget);
    m_settingsWidget->setFixedWidth(250);
    m_settingsWidget->hide();
    m_settingsWidget->layout()->setContentsMargins(0,0,0,0);

    mc->layout()->addWidget(m_settingsWidget);
    mc->layout()->addWidget(m_console=new ConsoleWidget);
    c->layout()->addWidget(mc);
    m_console->setTent(m_tent);
    m_console->setEnableConsole(false);


    m_stack->addWidget(m_overview=new ConfigOverview);
    m_overview->setTent(m_tent);
    m_stack->addWidget(m_tentEdit=new UnitEditor(this));
    m_stack->addWidget(m_archives=new ArchiveWidget);
    prepareSelector();
    setCentralWidget(c);

    connect(m_tentEdit,SIGNAL(editDeviceRequest(Device*)),this,SLOT(editDevice(Device*)));
    connect(m_tentEdit,SIGNAL(editParameterRequest(Parameter*)),this,SLOT(editParam(Parameter*)));
    connect(m_devices,SIGNAL(edit(int)),this,SLOT(deviceTrigSlot(int)));
    connect(m_selector,SIGNAL(triggered(int)),this,SLOT(goToIndex(int)));

    goToIndex(General);



   // settings();
    archive();



}

void GrowingTent::loadStyle()
{
    setStyleSheet("");




    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(49, 44, 64));
    darkPalette.setColor(QPalette::WindowText, QColor(215, 213, 218));
    darkPalette.setColor(QPalette::Base, QColor(44, 40, 58));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipText, QColor(215, 213, 218));

    // Text colors
    darkPalette.setColor(QPalette::Text, QColor(215, 213, 218));
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::PlaceholderText, QColor(127, 127, 127));

    // Button colors
    darkPalette.setColor(QPalette::Button, QColor(154, 0, 181));//main color
    darkPalette.setColor(QPalette::ButtonText  , QColor(215, 213, 218));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));

    // Highlight colors
    darkPalette.setColor(QPalette::Highlight, QColor(45, 169, 92));// main
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Link, QColor(77, 58, 172));

    // Additional colors (optional)
    darkPalette.setColor(QPalette::Dark, QColor(35, 35, 35));
    darkPalette.setColor(QPalette::Shadow, QColor(20, 20, 20));
    darkPalette.setColor(QPalette::BrightText,
     Qt::red);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));

    qApp->setStyle(QStyleFactory::create("Fusion"));
    // setStyleSheet(ChatBot::loadResourceFile("://styleSheet"));

    qApp->setPalette(darkPalette);
}

HardwareUnit *GrowingTent::currentUnit()
{
    int i=m_selector->currentIndex();
    if(i>=Hardware)
    {
        HardwareUnit* u=m_tent->units()[i-Hardware];
        qDebug()<<"currentUnit"<<u->name();
        return u;
    }

    return nullptr;
}

void GrowingTent::prepareSelector()
{
    QStringList ls;
    ls<<"Général";

    auto ld=m_tent->units();
    for(int i=0;i<ld.count();i++)
    {
        ls<<ld[i]->name();
    }
    ls<<"Récoltes";
    m_selector->setActions(ls);
}

void GrowingTent::showSettings(bool s)
{
    m_settingsWidget->setHidden(!s);
    m_settings->setChecked(s);
    if(s)
    {
        if(currentUnit())
            m_devices->fillList(currentUnit());
        else
            m_devices->fillList(m_tent->devices());
    }
    else
    {
        m_devices->clear();
    }

   // m_parameter->setHidden(true);
   // m_parameter->setPlot(nullptr);
}

void GrowingTent::goToIndex(int i)
{

    if(i==m_selector->list().count()-1)
    {
        m_stack->setCurrentIndex(m_stack->count()-1);
        m_selector->setActive(m_selector->list().count()-1);
        return;
    }
    else if(i>=Hardware)
    {
        HardwareUnit*s=m_tent->units()[i-Hardware];
        m_stack->setCurrentIndex(Hardware);
        m_tentEdit->handle(s);

        if(m_settings->checked())
            m_devices->fillList(s);
    }
    else
    {
        m_stack->setCurrentIndex(General);

        if(m_settings->checked())
            m_devices->fillList(m_tent->devices());
    }



    m_selector->setActive(i);
}

void GrowingTent::help()
{
    bool b=m_console->enabledConsole();
    m_console->setEnableConsole(!b);
    m_help->setChecked(!b);
 /*
    QUrl fileUrl = QUrl::fromLocalFile(HELP_FILE);

       // Open the file using desktop services
       if (QDesktopServices::openUrl(fileUrl)) {
           qDebug() << "File opened successfully";
       } else {
           qDebug() << "Failed to open file";
       }
       */
}

void GrowingTent::editUnit(HardwareUnit *s)
{

    if(s)
    {
        goToIndex(m_tent->units().indexOf(s));
    }

    else
    {
        goToIndex(General);
    }
}



void GrowingTent::editParam(Parameter *)
{
    /*
    m_settingsWidget->setHidden(false);
    m_settings->setChecked(true);
    m_parameter->setHidden(false);
    m_parameter->setClient(p);
    m_parameter->setPlot(m_tentEdit->overview()->paramPlot(p));
    */

 //   auto l=m_tent->unitsForParameter(p);
  //  if(l.isEmpty())
  //      return;
  //  goToIndex(Hardware);


    // m_tentEdit->editParameter(p);
}

void GrowingTent::editDevice(Device *d)
{
    showSettings(true);
    m_devices->setChecked(d);    
}


void GrowingTent::quitSlot()
{
    QApplication::quit();
}

void GrowingTent::settingsSlot()
{
    bool s=m_settings->checked();
    showSettings(!s);
}

void GrowingTent::deviceTrigSlot(int )
{

    //react to device clicked in the list
}

void GrowingTent::settings()
{
    SetupWizzard* setup=new SetupWizzard(m_tent,this);
    setup->start();
}

void GrowingTent::archive()
{
    auto a=ArchiveWizzard::archive(m_tent,this);
    connect(a,SIGNAL(archived(Archive)),this,SLOT(archivedSlot()));
}

void GrowingTent::archivedSlot()
{
     m_archives->load();

}

