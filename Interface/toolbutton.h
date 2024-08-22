#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include "actionwidget.h"

class QPushButton;
class QLabel;

class ToolButton : public ActionWidget
{
    Q_OBJECT
public:
    typedef enum layoutMode{horizontal, vertical, iconOnly,textOnly}LayoutMode;
    explicit ToolButton(QString title= QString(), QString Icon=QString(), QWidget *parent = nullptr);
    void setMovie(QString s);
    void setMovie(QMovie*m);
    void setIcon(QString s);
    void setIcon(QIcon i);
    void setText(QString);
    void setFontSize(int i);
    void setIconSize(QSize);
    void setIconSize(int s);
    void setLayoutMode(LayoutMode m);
    void setRound(int diameter=50, int contentmargins=0);

    QString title();
    QMovie *movie() const;

signals:

public slots:

protected:
    QMovie* m_movie;
    QLabel* m_icon;
    QLabel* m_title;
    LayoutMode m_layoutMode;
};
class ToolButtonText : public ToolButton
{
    Q_OBJECT
public:

    explicit ToolButtonText(QWidget *parent = nullptr);
    void setSubText(QString );
    QString subText();

signals:

public slots:

protected:
    QLabel* m_subText;

};


class ToolCenterButton : public ToolButtonText
{
    Q_OBJECT
public:
    explicit ToolCenterButton(QWidget *parent = nullptr);

signals:

public slots:

private:
};


#endif // TOOLBUTTON_H
