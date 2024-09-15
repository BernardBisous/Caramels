#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include "actionwidget.h"
#include "qscrollbar.h"
#include <QScrollArea>

class ScrollBarVerical : public QScrollBar
{
    Q_OBJECT
public:
    explicit ScrollBarVerical(QWidget *parent = nullptr);


    void setHover(bool newHover);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool m_hover;
};

class ScrollBarHorizontal : public QScrollBar
{
    Q_OBJECT
public:
    explicit ScrollBarHorizontal(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};
class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:


    explicit ScrollArea(bool horizontal=false,QWidget *parent = nullptr);
    virtual void enterEvent(QEnterEvent *event);
    virtual void leaveEvent(QEvent *event);
    void setHorizontal(bool b=true);
    void clear();
    void fillList(QStringList s);
    void addWidget(QWidget *w);
    void addActionText(QString s);
    void addActionWidget(ActionWidget*a);
    void addActionText(QString text, QString icon);
    void addSpacer();
    void resizeEvent(QResizeEvent *event) override;
    QWidget* at(int i);
    int indexOf(QWidget*w);
    QList<QWidget*> widgets();
    void scrollDown();

signals:
    void trigger(int i, QWidget*);

private slots:
    void trigSlot();
private:
    bool m_horizontal;
};

#endif // SCROLLAREA_H
