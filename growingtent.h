#ifndef GROWINGTENT_H
#define GROWINGTENT_H

#include "Interface/drawerselector.h"
#include "Interface/overview.h"
#include "configeditor.h"
#include "tent.h"
#include "tenteditor.h"
#include "webcamwidget.h"
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

    TentEditor* m_tentEdit;
    GrowConfig* m_currentConfig;
    Tent* m_tent;
    QStackedWidget* m_stack;
    DrawerSelector* m_selector;
    WebcamWidget* m_webcam;
    QLabel*m_nameLab;
    Overview* m_overview;
    ConsoleWidget* m_console;
    WebcamWidget* m_cam;
};

#endif // GROWINGTENT_H
