#include "unitoverviews.h"

WaterOverview::WaterOverview(QWidget*parent): UnitOverview(parent)
{

}

void WaterOverview::handle(HardwareUnit *u)
{
    m_waterManager=dynamic_cast<WaterLevelManager*>(u);
    UnitOverview::handle(u);
}

void WaterOverview::refreshCentral()
{

}


