#ifndef GROWINGTENT_H
#define GROWINGTENT_H

#include "Interface/drawerselector.h"
#include "configeditor.h"
#include "tent.h"
#include "tenteditor.h"
#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>

class GrowingTent : public QMainWindow
{
    Q_OBJECT
public:
    explicit GrowingTent(QWidget* parent=nullptr);
    void loadStyle();

public slots:
    void goToIndex(int i);
    void help();
    void editUnit(HardwareUnit*s);
    void editParam(Parameter*p);

signals:

private:
    ConfigEditor* m_editor;
    TentEditor* m_tentEdit;
    GrowConfig* m_currentConfig;
    Tent* m_tent;
    QStackedWidget* m_stack;
    DrawerSelector* m_selector;
    QLabel*m_nameLab;
};

#endif // GROWINGTENT_H
