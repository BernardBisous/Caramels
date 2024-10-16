#include "setupwizzard.h"
#include "qlabel.h"
#include "qlayout.h"


SetupWizzard::SetupWizzard(Tent *t, QWidget *parent)
    : Wizzard{parent},m_client(t)
{

    if(t&&t->needsInstall())
    {
        addSequence(m_welcome=new WelcomeSequence(this));
    }


    addSequence(m_mode=new ModeSequence(t));
    createWatering();
    createInjector();







}

void SetupWizzard::createInjector()
{
    addSequence(m_injector=new WizzardChoice(this));
    m_injector->setTexts("Do you have the PH regulator device ?",
                    "You can purshase it in one of our retailer or make one using the open source files");

    QStringList li,si,ss;
    li<<"Manual Dosing";
    ss<<"Use pure water or perform the chemichal dosing yourself";
    si<<":/icons/conventionnal";

    li<<"PH regulation device";
    ss<<"Monitor and regulate PH automatically (requieres the WT3000 extention )";
    si<<":/icons/aqua";

    li<<"Full robot injector";
    ss<<"Monitor and regulate all of the good sauces automatically (Requieres the Injector device)";
    si<<":/icons/aero";
    m_injector->setName("Produits");

    m_injector->setActions(li,ss,si);
}

void SetupWizzard::createDevices()
{
    if(!m_client)
        return;

    auto ls=m_client->externalDevices();

    for(int i=0;i<ls.count();i++)
    {
        DeviceSequence*s=new DeviceSequence(ls[i],m_client,this);
        addSequence(s);
    }
}

void SetupWizzard::createWatering()
{
    addSequence(m_water=new WizzardChoice(this));
    m_water->setName("Arrosage");

    m_water->setTexts("What it your watering system ?",
                    "You can purshase it in one of our retailer or make one using the open source files");

    QStringList li,si,ss;
    li<<"Direct injection";
    ss<<"Use pure water or perform the chemichal dosing yourself";
    si<<":/icons/conventionnal";

    li<<"Unique tank";
    ss<<"Monitor and regulate PH automatically (requieres the WT3000 extention )";
    si<<":/icons/aqua";

    li<<"Full robot injector";
    ss<<"Monitor and regulate all of the good sauces automatically (Requieres the Injector device)";
    si<<":/icons/aero";



    m_water->setActions(li,ss,si);
}

void SetupWizzard::reactToSequenceFinished(WizzardSequence *s, WizzardResult r)
{

    if(s==m_injector)
    {
        int sr=m_injector->result();
        m_client->setChemicalMode(Tent::ChemicalMode (sr));

        createDevices();
    }

}

WelcomeSequence::WelcomeSequence(QWidget *parent)
     : WizzardSequence{true,parent}
{
    QFont f=font();
    f.setPointSize(30);
    m_title->setFont(f);
    m_name="Bienvenue";
    m_isWelcome=true;
    setTexts("Are you ready to grow some real shit ?","You are a few steps away to start the first batch, here is what you need here:\n - All your tent stuff to install\n - flat screwdriver\n - A multimeter ?\nThis setup may take about an hour");
}


ModeSequence::ModeSequence(Tent *t, QWidget *parent):
    WizzardChoice{parent},m_client(t)
{
    m_name="Mode de culture";
    setTexts("In what mode do you intent to use the tent ?",
                    "You may have to modify the kit by yourself just a little bit");

    QStringList li,si,ss;
    li<<"Conventionnal";
    ss<<"Grow your plants in the dirt with an automated watering";
    si<<":/icons/conventionnal";

    li<<"Aquapony";
    ss<<"Give your plant the perfect pool to be controlled finelly";
    si<<":/icons/aqua";

    li<<"Aeropony";
    ss<<"Grow your plants in the real cloud of nutriments";
    si<<":/icons/aero";

    setActions(li,ss,si);
}

void ModeSequence::refreshButtons()
{
    Tent::GrowingMode m=m_client->currentGrowingMode();
    select(m);
}

void ModeSequence::reactToSelected(int i)
{
    m_client->setGrowingMode(Tent::GrowingMode(i));
    refreshButtons();
}

DeviceSequence::DeviceSequence(Device *d, Tent *t, QWidget *parent):
    WizzardSequence(false,parent),m_client(t),m_device(d),m_confirmed(false),m_measured(false)
{
    m_name=d->name();

    QWidget*w =new QWidget;
    w->setLayout(new QHBoxLayout);
    w->layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(w);

    w->layout()->addWidget(m_power=new QLabel("Acquiring.."));
    w->layout()->addWidget(m_value=new QLabel("0"));
    m_power->setHidden(true);
    m_value->setHidden(true);

    QWidget*ws=new QWidget;
    ws->setLayout(new QHBoxLayout);
    ws->layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(ws);
    ws->layout()->addWidget(m_plot=new DevicePlot);
    ws->layout()->setSpacing(20);




    QWidget* we=new QWidget;
    we->setLayout(new QVBoxLayout);
    we->layout()->setContentsMargins(0,0,0,0);
    QLabel* ediTex=new QLabel("Below is the device editor, access it from the top left corner to control the device directly");
    ediTex->setWordWrap(true);
    we->layout()->addWidget(ediTex);
    we->layout()->addWidget(m_editor=new DeviceEditor);

    QWidget*wes=new QWidget;
    m_plot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    m_plot->setMaximumHeight(300);
    wes->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    we->layout()->addWidget(wes);

    ws->layout()->addWidget(we);

    ediTex->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    layout()->addWidget(m_powerPlot=new DevicePlot);
    m_powerPlot->setHidden(true);


    m_editor->setMinimumWidth(200);
    addMainButton(m_validation=new ToolButton("Confirm it works"));
    m_validation->setCheckable(true);
    m_validation->setMode(ActionWidget::normal);

    connect(m_validation,SIGNAL(clicked()),this,SLOT(confirmSlot()));

    m_tester=new DeviceTester(d,t,this);
    connect(m_tester,SIGNAL(newCurrent(float)),this,SLOT(valueCurrent(float)));

    QFont f=font();
    f.setPointSize(18);
    m_power->setFont(f);
    m_value->setFont(f);

    m_value->setAlignment(Qt::AlignRight);
    setTexts(d->name(),m_tester->subText());
}

void DeviceSequence::start()
{
    WizzardSequence::start();
    m_plot->handle(m_device);
    m_powerPlot->handle(m_tester->sensor());
    m_plot->initStyle();
    m_plot->updatePlot();
    m_plot->setRealTime(true);
    m_editor->handle(m_device);
    m_confirmed=false;
    m_measured=false;
    m_validation->setChecked(false);
    m_plot->setXRangeSecs(5);

    connect(m_device,SIGNAL(newValue(float)),this,SLOT(valueSlot(float)));
    m_validation->setText("Caractériser");
    refresh();
}

void DeviceSequence::run()
{
    m_tester->run();
    m_validation->setText("...ça a l'air de fonctionner");
    refreshButtons();
}

void DeviceSequence::refreshButtons()
{
    if(m_nextButton)
     m_nextButton->setEnabled(m_confirmed && m_measured);

}

void DeviceSequence::refresh()
{
    m_value->setText(m_device->userValue());
}

void DeviceSequence::valueCurrent(float t)
{
    m_power->setText(QString::number(t,'f',1));
}

void DeviceSequence::valueSlot(float)
{
    refresh();
}

void DeviceSequence::confirmSlot()
{
    run();

}
