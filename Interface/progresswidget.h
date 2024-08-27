#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>
#include "progressbar.h"
#include "toolbutton.h"

class QLabel;
class QPushButton;
class QProgressBar;
class ActionWidget;
class TopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopWidget(QWidget *parent);
    virtual void start(int timeout=0);
    virtual void stop();
    void resetSize();
    void setImage(QString url, bool autoResize=true);
    void setImage(QPixmap p, bool autoResize=true);
    void setDeleteAtEnd(bool newDeleteAtEnd);
    void hideSpacers(bool s);

private slots:
    void timerSlot();
signals:
    void timeout();
    void closed();
    void started();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    QWidget* m_leftSpacer;
    QWidget* m_topSpacer;
    QWidget* m_rightSpacer;
    QWidget* m_bottomSpacer;
    QWidget* m_central;

private:
    QTimer* m_timer;
    QPixmap m_pix;
    bool m_autoresize;
    bool m_deleteAtEnd;



};

class TopGlobalWidget: public TopWidget
{
    Q_OBJECT
public:
    explicit TopGlobalWidget();

};



class ProgressWidget : public TopWidget
{
    Q_OBJECT
public:
    explicit ProgressWidget(QWidget *parent);

    void setEstimatedTime(int ms,float val=0);
    virtual void start(int timeout=0);
    virtual void stop();


    void setSubRichText();
    void addActions(QStringList l);
    ActionWidget* addAction(QString l,QString icon="");
    void addAction(ActionWidget* b);
    void clearActions();
    ActionWidget *addCancelAction(QString s="Cancel",QString icon=":/icons/kill");
    void setHoverVideo(QString s);
    void setTitleSize(int n);

    void setText(QString title,QString subTitle="");
    void setMovie(QString path,bool setName=true);
    void setMovie(QMovie *newMovie);
    void setPixmap(QPixmap p);
    void setMovieOnly();
    void setActionHorizontal(bool s);

    int defaultWidth() const;
    void setDefaultActionWidth(int newDefaultWidth);

private slots:
    void actionSlot();
    void actionHover(bool s);
    void cancelSlot();
    void progressTimeoutSlot();


signals:
    void actionTrigg(QString s, ActionWidget* e);
    void canceled();


protected:

   // QProgressIndicator* m_progress;
    QLabel* m_titleLabel;
    QLabel* m_subTextLabel;
    QLabel* m_videoLabel;
    QMovie* m_movie;
    QWidget* m_actionWidget;

    QString m_hoverVideo;
    QString m_videoPath;

    ProgressBar* m_bar;
    int m_defaultWidth;



};



class ProgressBarWidget: public TopWidget
{
    Q_OBJECT
public:
    explicit ProgressBarWidget(QWidget *parent);

    void startProgress(int toGet);
    void setState(QString s);
    void inc(QString s="");
    int toGet() const;
    int got() const;
    void setMovie(QMovie *newMovie);

public slots:
    virtual  void startSlot();
    virtual void stopSlot();





private:
    QLabel* m_stateLabel;
    QPushButton* m_startButton;
    QPushButton* m_cancel;
    QProgressBar* m_progress;
    QLabel* m_title;
    int m_got;
    int m_toGet;

};


class AcknowledgeWidget : public ProgressWidget
{
    Q_OBJECT
public:
    explicit AcknowledgeWidget(QWidget* parent);
    void show(QString title, QString sub=QString(), QString iconUrl=":/icons/danger", int ms=-1);

private slots:
    void closeSlot();

};


#endif // PROGRESSWIDGET_H
