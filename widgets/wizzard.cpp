#include "wizzard.h"
#include "Interface/toolbutton.h"
#include "qboxlayout.h"
#include "qdir.h"
#include "qgraphicsitem.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qlabel.h"
#include "qtimer.h"

#define READ_ME "readMe"
#define SEQ_SEP "&&"
#define DAT_SEP ";"

#define WRITE_DELAY 50
#define SUB_DELAY 500

#include "constants.h"

Wizzard::Wizzard(QWidget *parent)
    : TopWidget{parent},m_deleteAtEnd(false)
{

    m_central->setLayout(new QVBoxLayout);


    QWidget* buttons=new QWidget;
    buttons->setLayout(new QHBoxLayout);
    buttons->layout()->addWidget(m_backButton=new ToolButton("Back",":/icons/left"));
    buttons->layout()->addWidget(m_title=new QLabel);
    buttons->layout()->setContentsMargins(5,30,5,5);


    m_title->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_title->setAlignment(Qt::AlignCenter);
    m_backButton->setRound();


    QFont f=font();
    f.setPointSize(18);
 //   m_title->setAlignment(Qt::AlignCenter);
    m_title->setFont(f);


    auto sl=static_cast<QBoxLayout*>(m_middleWidget->layout());
    sl->insertWidget(0,buttons);

    m_central->layout()->addWidget(m_stack=new QStackedWidget);

    QWidget* okBar=new QWidget;
    okBar->setLayout(new QHBoxLayout);
    QWidget*ls=new QWidget;
    ls->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    okBar->layout()->addWidget(ls);
    okBar->layout()->addWidget(m_nextButton=new ToolButton("Next",":/icons/right"));
    m_nextButton->setRadius(25);
    m_nextButton->layout()->setSpacing(10);
    m_nextButton->layout()->setContentsMargins(5,5,15,5);
     m_nextButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    m_middleWidget->layout()->addWidget(okBar);
    m_leftSpacer->setLayout(new QHBoxLayout);
    m_leftSpacer->layout()->addWidget(m_list=new ScrollArea);
    m_list->removeMargins();
    m_leftSpacer->layout()->addWidget(m_connectingView= new QGraphicsView);
    m_connectingView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_connectingView->setBackgroundRole(QPalette::Window);
    m_connectingView->setScene(m_connectingScene=new QGraphicsScene);
    m_connectingView->setRenderHint(QPainter::Antialiasing);
    m_connectingView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_connectingView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_connectingItem=new QGraphicsPathItem;
    m_connectingScene->addItem(m_connectingItem);
    m_connectingView->setFixedWidth(100);

    m_leftSpacer->layout()->setContentsMargins(30,30,30,30);
    m_leftSpacer->layout()->setSpacing(0);


    connect(m_backButton,SIGNAL(clicked()),this,SLOT(back()));
    connect(m_nextButton,SIGNAL(clicked()),this,SLOT(next()));
    connect(m_list,SIGNAL(trigger(int,QWidget*)),this,SLOT(listTrig(int,QWidget*)));



    m_closeButton=new ToolButton("Close",":/icons/delete",this);
    connect(m_closeButton,SIGNAL(clicked()),this,SLOT(close()));

    m_central->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

}

void Wizzard::start(int timeout)
{

    TopWidget::start(timeout);

    placeCloseButton();




   // m_list->fillList(names());

    execute();
}

void Wizzard::reactToSequenceFinished(WizzardSequence *s, WizzardResult r)
{

}

void Wizzard::load(QString filePath,QString prefix)
{
        QFile jsonFile(prefix+filePath);

       if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

           return;
       }

       // Read the contents of the JSON file into a QByteArray
       QByteArray jsonData = jsonFile.readAll();
       jsonFile.close();

       // Parse the JSON data into a QJsonDocument
       QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

       // Check if the JSON data is valid
       if (jsonDoc.isNull()) {
           return ;
       }

       // Access the JSON object from the document
       QJsonObject jsonObject = jsonDoc.object();
       load(jsonObject,prefix);
}

void Wizzard::finish()
{
    emit finished();

    if(m_deleteAtEnd)
        delete this;
}

void Wizzard::next()
{
    auto a=currentSequence();
    if(!a)
        return;

    a->finish();

}

void Wizzard::back()
{
    int i=m_stack->currentIndex();
    if(i>0)
        startSequence(i-1);
    else
    {
       close();
    }
}

void Wizzard::execute()
{
    startSequence(0);
}

void Wizzard::sequenceFinished(WizzardResult r)
{
    WizzardSequence*s=dynamic_cast<WizzardSequence*>(sender());
    if(!s)
        return;

    reactToSequenceFinished(s,r);

    int i=m_stack->currentIndex();
    if(i<m_stack->count()-1)
        startSequence(i+1);

    else
        finish();
}

void Wizzard::listTrig(int i, QWidget *)
{
    startSequence(i);
}

void Wizzard::buttonSlot()
{


}

void Wizzard::placeCloseButton()
{
    int m=20;
    int w=50;
    m_closeButton->setRound(w);
    QRect r(width()-w-m,m,w,w);
    m_closeButton->setGeometry(r);
    m_closeButton->show();
}

void Wizzard::addSequence(WizzardSequence *r)
{

    if(!r)
        return;

    connect(r,SIGNAL(result(WizzardResult)),this,SLOT(sequenceFinished(WizzardResult)));
    m_stack->addWidget(r);
    r->setButtons(m_backButton,m_nextButton);
    m_list->addActionText(r->name());

}

void Wizzard::startSequence(int i)
{
    if(i<0 || m_stack->count()<=i)
        return;

    WizzardSequence* s=dynamic_cast<WizzardSequence*>(m_stack->widget(i));

    if(s)
        startSequence(s);

}
void Wizzard::startSequence(WizzardSequence *s)
{
    int n=m_stack->indexOf(s);
    if(!s || n<0)
        return;

    m_title->setText(s->name());
    m_list->setCurrent(n);
    m_stack->setCurrentWidget(s);
    m_list->setHidden(n==0 && s->isWelcome());
    m_connectingView->setHidden(m_list->isHidden());
    redrawConnection();
    s->start();
}

bool Wizzard::deleteAtEnd() const
{
    return m_deleteAtEnd;
}

void Wizzard::setDeleteAtEnd(bool newDeleteAtEnd)
{
    m_deleteAtEnd = newDeleteAtEnd;
}

void Wizzard::setClosableButton(bool s)
{

    m_closeButton->setHidden(!s);
}

void Wizzard::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    placeCloseButton();
}



void Wizzard::load(QJsonObject a, QString prefix)
{
    QJsonArray sa=a.value("SEQUENCES").toArray();
    QString seq=a.value("SCHEDULE").toString();
    QStringList lid=seq.split("&");
     for(int i=0;i<lid.count();i++)
    {
        int id=lid[i].toInt();
        addSequence(sequenceFactory(sa,id,prefix));
    }
}

WizzardSequence *Wizzard::sequenceFactory(QJsonArray sa, int id, QString prefix)
{
    for(int i=0;i<sa.count();i++)
    {
        QJsonObject ob=sa[i].toObject();

        int ids=ob.value("ID").toInt();
        int type=ob.value("TYPE").toInt();
        if(ids==id)
        {
           WizzardSequence* s=sequenceFactory(type);
           if(s)
           {
               s->load(ob,prefix);
               return s;
           }
        }
    }
    return nullptr;
}

WizzardSequence *Wizzard::sequenceFactory(int id)
{
    switch(id)
    {
    case NormalSequenceType: return new WizzardSequence(true);
    case ChoiceSequenceType: return new WizzardChoice;
    case ValueSequenceType: return new WizzardValue;
    case WebcamSequenceType: return new WizzardCapture(nullptr);// change to real webcam
    default:return nullptr;
    }
}

WizzardSequence *Wizzard::currentSequence()
{
    return dynamic_cast<WizzardSequence*>(m_stack->currentWidget());
}


QStringList Wizzard::names()
{
    QStringList out;
    for(int i=0;i<m_stack->count();i++)
    {
        out<<static_cast<WizzardSequence*>(m_stack->widget(i))->name();
    }
    return out;

}

QList<WizzardSequence *> Wizzard::sequences()
{
    QList<WizzardSequence *> out;
    for(int i=0;i<m_stack->count();i++)
    {
        WizzardSequence* c=static_cast<WizzardSequence*>(m_stack->widget(i));
        if(c)
            out<<c;
    }
    return out;
}

void Wizzard::redrawConnection()
{
    QPainterPath p;

    QWidget* as=m_list->currentAction();
    QWidget* bs=m_backButton;

    QPoint ap=as->mapToGlobal(as->rect().center()+QPoint(as->width()/2,0));
    QPoint bp=bs->mapToGlobal(bs->rect().center()-QPoint(bs->width()/2,0));

    ap=m_connectingView->mapFromGlobal(ap);
    bp=m_connectingView->mapFromGlobal(bp);

    ap=QPoint(0,ap.y());
    bp=QPoint(width(),bp.y());

    p.moveTo(ap);
    p.lineTo(ap.x() + 20, ap.y());
    p.lineTo(bp.x() - 20, bp.y());
    p.lineTo(bp.x(), bp.y());

    m_connectingItem->setPen(QPen(palette().highlight().color()));
    m_connectingItem->setPath(p);
}

void Wizzard::close()
{
    emit canceled();
    if(m_deleteAtEnd)
        delete this;
}

WizzardSequence::WizzardSequence(bool anim, QWidget *parent):
    QWidget(parent),m_titleToWrite(""), m_writingTimer(nullptr),
    m_name("Sequence"),m_backButton(nullptr),m_nextButton(nullptr),m_isWelcome(false)
{
    setLayout(new QVBoxLayout);

    layout()->addWidget(m_titleWidget=new QWidget);

    m_titleWidget->setLayout(new QHBoxLayout);
    m_titleWidget->layout()->setContentsMargins(0,0,0,0);
    m_titleWidget->layout()->setSpacing(20);
    m_titleWidget->layout()->addWidget(m_title=new QLabel);
    m_title->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    layout()->setSpacing(20);
    layout()->addWidget(m_picture=new QLabel);
    layout()->addWidget(m_subtitle=new QLabel);

    m_picture->setAlignment(Qt::AlignCenter);


    loadPicture(":/Images/default");


    if(anim)
    {
        m_writingTimer=new QTimer(this);
        connect(m_writingTimer,SIGNAL(timeout()),this,SLOT(writeSlot()));
    }

    QFont f=font();
    f.setPointSize(24);
    m_title->setFont(f);


}

WizzardResult WizzardSequence::computeResult()
{
    WizzardResult out;
    out.sequence=m_name;
    return out;;
}

void WizzardSequence::start()
{
    if(m_writingTimer)
    {
        m_title->clear();
        m_writeCounter=0;
        m_writeSubs=false;
        m_writingTimer->start(WRITE_DELAY);
    }
}



void WizzardSequence::loadPicture(QString p)
{
    m_picture->setPixmap(QPixmap(p).scaledToWidth(400,Qt::SmoothTransformation));
}

QWidget *WizzardSequence::anchorWidget()
{
    return m_title;
}

void WizzardSequence::loadData(QString s)
{
    /*
       QStringList ls=s.split(DAT_SEP);
       if(ls.count()>=3)
       {
           setTexts(ls[0],ls[1]);
           loadPicture(ls[2]);
       }
       if(ls.count()>3)
       {
           QStringList ps,is,ss;
           for(int i=3;i<ls.count()-3;i++)
           {
               ps<<ls[(i-3)*3];
               is<<ls[(i-3)*3+1];
               ss<<ls[(i-3)*3+2];
           }
           setActions(ps,ss,is);
       }
       */
}

void WizzardSequence::load(QJsonObject a,QString prefix)
{
    m_name=a.value("NAME").toString();
    QString st=a.value("TITLE").toString();
    QString sb=a.value("DESC").toString();
    setTexts(st,sb);



    QString pix=a.value("PICTURE").toString();
    if(!pix.isEmpty())
        loadPicture(prefix+pix);
}

void WizzardSequence::addWidget(QWidget *w)
{
    layout()->addWidget(w);
}

void WizzardSequence::setTexts(QString title, QString sub)
{


    if(m_writingTimer)
    {
          m_writeCounter=0;
          m_writeSubs=false;
          m_titleToWrite=title;
          m_subToWrite=sub;
    }
    else
    {
        m_subtitle->setText(sub);
        m_title->setText(title);
    }
}

WizzardChoice::WizzardChoice(QWidget *parent):
    WizzardSequence(parent),m_result(-1),m_autofnish(true)
{
    layout()->addWidget(m_buttons=new QWidget);
    m_buttons->setLayout(new QVBoxLayout);
}

void WizzardChoice::setActions(QStringList ls, QStringList subTexts, QStringList subIcons)
{
    while(m_buttonsList.count())
    {
        auto i=m_buttonsList.takeAt(0);
        if(i)
            delete i;
    }
    m_buttonsList.clear();


    for(int i=0;i<ls.count() && i<subIcons.count();i++)
    {
        ToolButtonText* bs=new ToolButtonText(ls[i],subIcons[i],subTexts[i],this);
        m_buttons->layout()->addWidget(bs);
        connect(bs,SIGNAL(clicked()),this,SLOT(buttonSlot()));
        m_buttonsList<<bs;
    }
}

WizzardResult WizzardChoice::computeResult()
{
    WizzardResult a;
    a.sequence=name();
    a.result=QString::number(m_result);
    return a;
}

void WizzardChoice::start()
{
   WizzardSequence::start();
   refreshButtons();



}

void WizzardChoice::refreshButtons()
{
    if(m_nextButton)
    {
        m_nextButton->setEnabled(m_result>-1);
    }
}

void WizzardChoice::reactToSelected(int i)
{

}



void WizzardSequence::finish()
{

    if(m_writingTimer)
        m_writingTimer->stop();

    emit result(computeResult()); 
}

void WizzardSequence::writeSlot()
{
    if(!m_writingTimer)
        return;

    if(!m_writeSubs)
    {
        QString se=m_titleToWrite;
        se.resize(m_writeCounter++);
        m_title->setText(se);

        if(m_writeCounter>m_titleToWrite.size())
        {
            m_writeSubs=true;
            m_writeCounter=0;
            m_subtitle->clear();
            m_subtitle->show();
            m_writingTimer->setInterval(SUB_DELAY);
        }
    }

    else{

        QString se=m_subToWrite;
        QStringList sel=se.split("\n",Qt::SkipEmptyParts);
        sel.resize(m_writeCounter++);
        m_subtitle->setText(sel.join("\n"));

        if(m_writeCounter>m_subToWrite.split("\n",Qt::SkipEmptyParts).count())
        {
            m_writeSubs=true;
            m_writingTimer->stop();
        }
    }
}

bool WizzardSequence::isWelcome() const
{
    return m_isWelcome;
}

QString WizzardSequence::name() const
{
    return m_name;
}

void WizzardSequence::setButtons(ToolButton *back, ToolButton *next)
{
    m_backButton=back;
    m_nextButton=next;
}

void WizzardSequence::addMainButton(ToolButton *b)
{
    m_titleWidget->layout()->addWidget(b);
    b->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
}

void WizzardSequence::setName(const QString &newName)
{
    m_name = newName;
}

void WizzardChoice::buttonSlot()
{
    QObject*ob=sender();

    if(!ob)
        return;

    int i=m_buttonsList.indexOf(ob);
    select(i);

    if(m_autofnish)
        finish();
}

bool WizzardChoice::autofnish() const
{
    return m_autofnish;
}

void WizzardChoice::setAutofnish(bool newAutofnish)
{
    m_autofnish = newAutofnish;
}

int WizzardChoice::result() const
{
    return m_result;
}

void WizzardChoice::select(int index)
{
    m_result=index;

    for(int i=0;i<m_buttonsList.count();i++)
    {
        m_buttonsList[i]->setChecked(i==index);
    }

    if(m_nextButton)
    {
        m_nextButton->setEnabled(index>-1);
    }
}

WizzardValue::WizzardValue(QString valName,QString valDefaul,QString desc,QString units,QWidget *parent)
    :WizzardSequence(false,parent)
{

    addWidget(m_value=new QLineEdit);

    QFont f=font();
    f.setPointSize(18);
    m_value->setFont(f);
    setTexts("Quel est le "+valName+" ?",desc);
    setName(valName);

    m_value->setPlaceholderText("Enter the value in "+units);
    m_value->setText(valDefaul);
    connect(m_value, SIGNAL(editingFinished()),this,SLOT(editSlot()));
}

QString WizzardValue::stringValue()
{
    return m_value->text();
}

WizzardResult WizzardValue::computeResult()
{
    WizzardResult out;
    out.result=m_value->text();
    out.sequence=name();
    return out;
}

void WizzardValue::editSlot()
{
    finish();
}

WizzardCapture::WizzardCapture(Webcam *w, QWidget *parent):
   WizzardSequence(false,parent)
{
    m_name="Capture";
    setTexts("Immortalisez ce moment","Approchez la web cam et cliquez pour capturer");
    addWidget(m_camWidget=new WebcamWidget);
    m_picture->hide();
    m_camWidget->handle(w);
    connect(w,SIGNAL(saved(QString)),this,SLOT(savedSlot(QString)));
}

WizzardResult WizzardCapture::computeResult()
{
    auto a=WizzardSequence::computeResult();
    a.result=m_path;
    return a;

}

QPixmap WizzardCapture::pixmapResult()
{
    return QPixmap(m_path);
}

void WizzardCapture::savedSlot(QString s)
{

   m_path=s;
   setTexts("C'est joli?","Passez à l'étape suivante ou cliquez sur l'image pour la refaire");

   finish();

}


