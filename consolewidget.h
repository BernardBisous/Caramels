#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H


#include "Interface/scrollarea.h"
#include "tent.h"
#include <QWidget>
#include <QLabel>
class ConsoleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConsoleWidget(QWidget *parent = nullptr);
    void setTent(Tent*t);



    void setEnableConsole(bool newEnable);

    bool enabledConsole() const;

public slots:
    void append(QString s);
    void clear();
void reset();

signals:

private:


    bool m_enable;
    Tent* m_tent;
    ScrollArea* m_scroll;
    QLabel* m_text;
};

#endif // CONSOLEWIDGET_H
