#ifndef UNITOVERVIEWS_H
#define UNITOVERVIEWS_H


#include "unitoverview.h"
#include "hardware/waterlevelmanager.h"
class WaterOverview : public UnitOverview
{
    Q_OBJECT
public:
    explicit WaterOverview (QWidget *parent = nullptr);
    virtual void handle(HardwareUnit*u);
    virtual void refreshCentral();


signals:

private:
    WaterLevelManager* m_waterManager;
    HardwareUnit* m_client;
    QGridLayout* m_grid;

    QList<ParameterPlot*> m_parameters;
    IntegralPlot* m_integral;
    QList<DevicePlot*> m_devices;
};
#endif // UNITOVERVIEWS_H
