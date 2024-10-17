#ifndef WIZZARD_H
#define WIZZARD_H

#include "Interface/progresswidget.h"
#include "Interface/scrollarea.h"
#include "Interface/toolbutton.h"
#include "hardware/webcam.h"
#include "qgraphicsview.h"
#include "qlineedit.h"
#include "qstackedwidget.h"
#include "widgets/webcamwidget.h"

#include <QWidget>

class WizzardResult
{
public:
    QString sequence;
    QString result;
};



class WizzardSequence: public QWidget
{
    Q_OBJECT
public:


    explicit WizzardSequence(bool anim=false,QWidget *parent = nullptr);
    virtual WizzardResult computeResult();
    virtual void start();
    void loadPicture(QString p);
    QWidget* anchorWidget();
    void loadData(QString s );
    void load(QJsonObject a,QString prefix);
    void addWidget(QWidget* w);


    void setTexts(QString title,QString sub);
    virtual void finish();
    void setName(const QString &newName);
    QString name() const;
    void setButtons(ToolButton*back,ToolButton* next);
    void addMainButton(ToolButton* b);

    bool isWelcome() const;

signals:
    void result(WizzardResult r);

private slots:
    void writeSlot();

protected:
    QLabel* m_title;
    QLabel* m_subtitle;
    QLabel* m_picture;



    QTimer* m_writingTimer;
    QString m_name;
    ToolButton* m_backButton;
    ToolButton* m_nextButton;
    QWidget* m_titleWidget;

    QString m_titleToWrite;
    QString m_subToWrite;
    bool m_isWelcome;
    int m_writeCounter;
    bool m_writeSubs;

};

class WizzardValue: public WizzardSequence
{
    Q_OBJECT
public:
    explicit WizzardValue(QString valName="", QString valDefaul="", QString description="", QString units="", QWidget *parent = nullptr);
    void setUnits(QString s);
    QString stringValue();
    virtual WizzardResult computeResult();

private slots:
    void editSlot();

private:
    QLineEdit* m_value;
    QLabel* m_units;
};

class WizzardCapture: public WizzardSequence
{
    Q_OBJECT
public:
    explicit WizzardCapture(Webcam* w,QWidget*parent=nullptr);
    virtual WizzardResult computeResult();
    QPixmap pixmapResult();

private slots:
    void savedSlot(QString s);
private:
    WebcamWidget* m_camWidget;
    Webcam* m_cam;
    QString m_path;


};

class WizzardChoice: public WizzardSequence
{
    Q_OBJECT
public:
    explicit WizzardChoice(QWidget *parent = nullptr);
    void setActions(QStringList ls,QStringList subTexts=QStringList(),QStringList subIcons=QStringList());
    virtual WizzardResult computeResult();
    virtual void start();
    virtual void refreshButtons();
    virtual void reactToSelected(int i);
    int result() const;
    void select(int i);


    bool autofnish() const;
    void setAutofnish(bool newAutofnish);

protected slots:
   void buttonSlot();

private:
   int m_result;
   QWidget* m_buttons;
   bool m_autofnish;
   QList<ToolButton*> m_buttonsList;
};


class Wizzard : public TopWidget
{
    Q_OBJECT
public:
    explicit Wizzard(QWidget *parent = nullptr);
    virtual void start(int timeout=0);
    virtual void reactToSequenceFinished(WizzardSequence*s,WizzardResult r);
    void load(QString sPath,QString prefix);
    virtual void finish();
    void addSequence(WizzardSequence*r);
    void startSequence(int i);
    void setClosable(bool newClosable);
    void resizeEvent(QResizeEvent *event) ;
    void load(QJsonObject a,QString prefix);
    WizzardSequence* sequenceFactory(QJsonArray o , int id,QString prefix);
    virtual WizzardSequence* sequenceFactory(int id);
    WizzardSequence* currentSequence();
    QStringList names();
    QList<WizzardSequence*> sequences();
    void redrawConnection();

    bool deleteAtEnd() const;
    void setDeleteAtEnd(bool newDeleteAtEnd);
    void setClosableButton(bool s);

public slots:
    void close();
    void next();
    void back();
    void execute();
    void sequenceFinished(WizzardResult r);
    void listTrig(int i,QWidget*w);

signals:
    void finished();
    void canceled();

private slots:
   void buttonSlot();

protected:
    QStackedWidget* m_stack;
    QLabel*m_title;
    ToolButton* m_backButton;
    ToolButton* m_nextButton;
    ToolButton* m_closeButton;
    void placeCloseButton();
    void startSequence(WizzardSequence*s);
    ScrollArea* m_list;
    QGraphicsScene* m_connectingScene;
    QGraphicsView* m_connectingView;
    QGraphicsPathItem* m_connectingItem;
    bool m_deleteAtEnd;
};


#endif // WIZZARD_H
