#include "growingtent.h"


#define HELP_FILE "help.pdf"
#include <QDesktopServices>
#include <QApplication>
#include <QStyleFactory>
enum {General=0,Hardware,Configs,Webcam};
GrowingTent::GrowingTent(QWidget* parent)
    : QMainWindow(parent)

{
    loadStyle();

    m_currentConfig=new GrowConfig;
    m_tent=new Tent;
    m_tent->setConfig(m_currentConfig);

    m_tent->begin();



    QWidget* bar=new QWidget;
    bar->setLayout(new QHBoxLayout);
    QLabel* pix;

    bar->layout()->addWidget(pix=new QLabel);
    pix->setPixmap(QPixmap(":/icons/logo").scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    bar->layout()->addWidget(m_nameLab=new QLabel(m_tent->name()));
    bar->layout()->addWidget(m_selector=new DrawerSelector);
    bar->layout()->setContentsMargins(0,0,0,0);



    QWidget* c=new QWidget;
    c->setLayout(new QVBoxLayout);
    c->layout()->addWidget(bar);

    QWidget* mc=new QWidget;
    mc->setLayout(new QHBoxLayout);
    mc->layout()->addWidget(m_stack=new QStackedWidget);
    mc->layout()->addWidget(m_console=new ConsoleWidget);
    c->layout()->addWidget(mc);
    m_console->setTent(m_tent);
    m_console->setEnableConsole(false);


    m_stack->addWidget(m_overview=new Overview);
    m_stack->addWidget(m_tentEdit=new TentEditor(this));
    m_stack->addWidget(m_cam=new WebcamWidget);
    m_cam->handle(m_tent->cam());

    QFont f=font();
    f.setPointSize(f.pointSize()+15);
    m_nameLab->setFont(f);

    m_selector->setActions(QStringList()<<"Général"<<"Hardware"<<"Webcam"); // dirty

    setCentralWidget(c);

    m_nameLab->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    connect(m_selector,SIGNAL(triggered(int)),this,SLOT(goToIndex(int)));
    connect(m_selector,SIGNAL(help()),this,SLOT(help()));


    connect(m_overview,SIGNAL(editOne(HardwareUnit*)),this,SLOT(editUnit(HardwareUnit*)));

    m_tentEdit->handle(m_tent);
    m_overview->loadHardware(m_tent);

    goToIndex(General);
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

void GrowingTent::goToIndex(int i)
{
    if(i==Hardware)
        m_tentEdit->edit(0);

    m_stack->setCurrentIndex(i);
    m_selector->setActive(i);
}

void GrowingTent::help()
{
    bool b=m_console->enabledConsole();
    m_console->setEnableConsole(!b);
    m_selector->helpButton()->setChecked(!b);
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
    goToIndex(Hardware);
    m_tentEdit->edit(s);
}



void GrowingTent::editParam(Parameter *p)
{

    auto l=m_tent->unitsForParameter(p);
    if(l.isEmpty())
        return;
    goToIndex(Hardware);
    m_tentEdit->edit(l.first());

}
