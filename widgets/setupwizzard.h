#ifndef SETUPWIZZARD_H
#define SETUPWIZZARD_H

#include <QWidget>
#include "hardware/devicetester.h"
#include "hardware/tent.h"
#include "widgets/deviceeditor.h"
#include "widgets/deviceplot.h"
#include "wizzard.h"


class WelcomeSequence : public WizzardSequence
{
    Q_OBJECT
public:
    explicit WelcomeSequence(QWidget *parent = nullptr);
};
class ModeSequence : public WizzardChoice
{
    Q_OBJECT
public:
    explicit ModeSequence(Tent* t,QWidget *parent = nullptr);
    virtual void reactToSelected(int i);
    virtual void refreshButtons();
private:
     Tent* m_client;
};

class DeviceSequence : public WizzardSequence
{
    Q_OBJECT
public:
    explicit DeviceSequence (Device*d,Tent* t,QWidget *parent = nullptr);
    void start();
    void run();
    virtual void refreshButtons();
    void refresh();

private slots:
    void valueCurrent(float t);
    void valueSlot(float t);
    void confirmSlot();

private:
     Tent* m_client;
     Device* m_device;
     DeviceTester* m_tester;
     DevicePlot* m_plot;
     DevicePlot* m_powerPlot;
     DeviceEditor * m_editor;
     QLabel* m_power;
    QLabel* m_value;
     ToolButton* m_validation;
     ActionWidget* m_state;
     bool m_confirmed;
     bool m_measured;
};


class SetupWizzard : public Wizzard
{
    Q_OBJECT
public:
    explicit SetupWizzard(Tent* t,QWidget *parent = nullptr);
    void createInjector();
    void createDevices();
    void createWatering();
    virtual void reactToSequenceFinished(WizzardSequence*s,WizzardResult r);


signals:

private:
    WelcomeSequence* m_welcome;
    WizzardChoice* m_injector;
    ModeSequence* m_mode;
    WizzardChoice* m_water;

    QList<WizzardSequence*> m_devices;
    Tent* m_client;

};

#endif // SETUPWIZZARD_H
