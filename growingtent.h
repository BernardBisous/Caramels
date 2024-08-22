#ifndef GROWINGTENT_H
#define GROWINGTENT_H

#include "configeditor.h"
#include "tent.h"
#include <QMainWindow>
#include <QWidget>

class GrowingTent : public QMainWindow
{
    Q_OBJECT
public:
    explicit GrowingTent();
    void loadStyle();

signals:

private:
    ConfigEditor* m_editor;
    GrowConfig* m_currentConfig;
    Tent* m_tent;
};

#endif // GROWINGTENT_H
