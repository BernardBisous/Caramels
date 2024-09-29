#ifndef GROWINGTENT_H
#define GROWINGTENT_H

#include "Interface/devicelistwidget.h"
#include "Interface/drawerselector.h"
#include "Interface/overview.h"
#include "configeditor.h"
#include "tent.h"
#include "uniteditor.h"
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
    HardwareUnit* currentUnit();
    void prepareSelector();
    void showSettings(bool s);

public slots:
    void goToIndex(int i);
    void help();
    void editUnit(HardwareUnit*s);
    void editParam(Parameter*p);
    void editDevice(Device*d);
    void quitSlot();
    void settingsSlot();
    void deviceTrigSlot(int i);

signals:

private:

    DeviceListWidget* m_devices;
    UnitEditor* m_tentEdit;
    GrowConfig* m_currentConfig;
    Tent* m_tent;
    QStackedWidget* m_stack;
    DrawerSelector* m_selector;
    WebcamWidget* m_webcam;
    QLabel*m_nameLab;
    ConfigOverview* m_overview;
    ConsoleWidget* m_console;
    HardwareOverview* m_tentOverview;
    ToolButton* m_help;
    ToolButton* m_quit;
    ToolButton* m_settings;
    QWidget* m_settingsWidget;
  //  ParameterValueEditor* m_parameter;

};

#endif // GROWINGTENT_H
