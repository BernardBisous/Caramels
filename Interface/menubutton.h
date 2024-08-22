#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QMenu>
#include <QAction>


class MenuButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MenuButton(QString dname="",QWidget *parent=nullptr);
    virtual void paintEvent(QPaintEvent *e) ;
    void nouvAction(QString iconUrl,QString text,QMenu* sub=nullptr);
    void addActions(QStringList,QMenu* m=nullptr);
    QAction* nouvAction(QString nam,QMenu* sub=nullptr);
    void setActivate(QString act);
    void setActivate(QAction* act);
    void setUniqueActiveAction(bool s=true);
    QStringList actionsList();
    QStringList actionsList(QMenu*m);
    QMenu* subMenu(QString path,QMenu* m_masterMenu=nullptr,QString iconUrl="");

    void disableAction(QString s);

    void clearActive(QMenu* m);

    QString actionAbsolutePath(QAction*a);


    QString findMenuIn(QMenu* m,QMenu* toFind);

    void triggAction(QAction* act);

    void refresh();

    QString textValue(bool absolutePath=true);

    QString menuPath(QMenu* a);

    void setDefaultText(const QString &defaultText);
    void setTitle(QString s);

    bool printActive() const;
    void setPrintActive(bool printActive);

    virtual void enterEvent(QEnterEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent*e);
    virtual void mouseReleaseEvent(QMouseEvent*e);

signals:
    void actionTrigg(QAction* act);
    void actionTrigg(QString path);
    void actionTrigg();

public slots:
    void clear();
    void actionHandle();




private:
    QString lastMenu;

    QAction* m_activeAction;

    QString m_defaultText;

    bool m_printActive;
    bool m_activeAtTop;
    bool m_uniqueActive;

    bool m_hover;
    bool m_press;


};

#endif // MENUBUTTON_H
