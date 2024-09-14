#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include "Interface/toolbutton.h"
#include "qtextedit.h"
#include "tent.h"
#include <QWidget>

class ConsoleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConsoleWidget(QWidget *parent = nullptr);
    void setTent(Tent*t);


public slots:
    void append(QString s);
    void clear();


signals:

private:
    QTextEdit* m_console;
};

#endif // CONSOLEWIDGET_H
