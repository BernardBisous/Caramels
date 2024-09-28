#ifndef DRAWERSELECTOR_H
#define DRAWERSELECTOR_H

#include "toolbutton.h"
#include <QWidget>
#include <QPushButton>


class DrawerSelectorItem: public QPushButton
{
    Q_OBJECT
    public:
    explicit DrawerSelectorItem(QString t,QWidget* parent=nullptr);
    virtual void paintEvent(QPaintEvent *event);
    void enterEvent(QEnterEvent * event);
    void leaveEvent(QEvent * event);
    bool m_hovered;

};


class DrawerSelector : public QWidget
{
    Q_OBJECT
public:
    explicit DrawerSelector(QWidget *parent = nullptr);
    void setActions(QStringList sList);
    void addAction(QString s);
    void remove(QString s);
    void setActive(int i);
    QString currentAction(int* i=nullptr);
    void hide(QStringList s,bool sb=true);
    void hideAll(bool s=true);
    QList<DrawerSelectorItem*> list();
    bool isHidden(int i);
    int currentIndex();
    bool nextIsHidden();
    void setHelpHidden(bool s);
    void disable(int i,bool s=true);
    ToolButton* helpButton(){return m_help;}



    ToolButton *settings() const;

signals:
    void triggered( int i);
    void changed( int i);
    void help();

private slots:
    void clickedSlot();
    void helpSlot();
    void quitSlot();

private:
    void clear();
    QStringList m_list;
    ToolButton* m_help;
    ToolButton* m_quit;
    ToolButton* m_settings;
    QWidget* m_actionWidget;
};



#endif // DRAWERSELECTOR_H
