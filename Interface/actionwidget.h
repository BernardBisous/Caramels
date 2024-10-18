#ifndef ACTIONWIDGET_H
#define ACTIONWIDGET_H

#include <QWidget>
#include <QPen>
class ActionWidget : public QWidget
{
    Q_OBJECT

public:
    typedef enum displayMode{normal,highlighted,noBorder,customPen, appearingBorder}DisplayMode;
    explicit ActionWidget(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *e) ;
    virtual void enterEvent(QEnterEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent*e);
    virtual void mouseReleaseEvent(QMouseEvent*e);

    virtual QBrush backgroundBrush();
    virtual QPen borderPen();
    void animateClick();

    void setAppear(bool newAppear);
    void setRadius(int newRadius);
    void setHover(bool newHover);

    void setCheckable(bool newCheckable);
    void setChecked(bool s=true);

    void setBorderPen(QPen p);

    bool hoverable() const;
    void setHoverable(bool newHoverable);

    DisplayMode mode() const;
    void setMode(DisplayMode newMode);

    bool invertText() const;
    void setInvertingText(bool newInvertText);

    void setHighlightText(bool s);


    bool ignoreClick() const;
    void setIgnoreClick(bool newIgnoreClick);

    bool checked() const;


signals:
    void clicked();
    void hovered(bool s);

protected:
    bool m_hover;
    bool m_press;
    bool m_appear;
    int m_radius;
    bool m_checked;
    bool m_checkable;
    bool m_hoverable;
    bool m_invertText;
    bool m_ignoreClick;
    QPen m_borderPen;
    DisplayMode m_mode;
};

#endif // ACTIONWIDGET_H
