#include "growingtent.h"


#define HELP_FILE "help.pdf"
#include <QDesktopServices>

enum{Hardware=0,Configs};
GrowingTent::GrowingTent()
    : QMainWindow{}

{
    loadStyle();

    m_currentConfig=new GrowConfig;
    m_tent=new Tent;
    m_tent->setConfig(m_currentConfig);

    m_tent->begin();



    QWidget* bar=new QWidget;
    bar->setLayout(new QHBoxLayout);
    bar->layout()->addWidget(m_nameLab=new QLabel(m_tent->name()));
    bar->layout()->addWidget(m_selector=new DrawerSelector);
    bar->layout()->setContentsMargins(0,0,0,0);



    QWidget* c=new QWidget;
    c->setLayout(new QVBoxLayout);
    c->layout()->addWidget(bar);
    c->layout()->addWidget(m_stack=new QStackedWidget);

    m_editor=new ConfigEditor(this);
    m_editor->handle(m_currentConfig);

    m_stack->addWidget(m_tentEdit=new TentEditor(this));
    m_stack->addWidget(m_editor);

    m_tentEdit->handle(m_tent);

    QFont f=font();
    f.setPointSize(f.pointSize()+15);
    m_nameLab->setFont(f);

    m_selector->setActions(QStringList()<<"Hardware"<<"Configurations"); // dirty

    setCentralWidget(c);

    m_nameLab->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    connect(m_selector,SIGNAL(triggered(int)),this,SLOT(goToIndex(int)));
    connect(m_selector,SIGNAL(help()),this,SLOT(help()));

    connect(m_editor,SIGNAL(editDevice(Device*)),this,SLOT(editDevice(Device*)));

    goToIndex(Hardware);




}

void GrowingTent::loadStyle()
{
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

    setPalette(darkPalette);
}

void GrowingTent::goToIndex(int i)
{
    m_stack->setCurrentIndex(i);
    m_selector->setActive(i);
}

void GrowingTent::help()
{
    QUrl fileUrl = QUrl::fromLocalFile(HELP_FILE);

       // Open the file using desktop services
       if (QDesktopServices::openUrl(fileUrl)) {
           qDebug() << "File opened successfully";
       } else {
           qDebug() << "Failed to open file";
       }
}

void GrowingTent::editDevice(Device *s)
{
    goToIndex(Hardware);
    m_tentEdit->edit(s);
}
