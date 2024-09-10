#ifndef PHMANAGER_H
#define PHMANAGER_H

#include <QObject>

#include "hardware/analogsensor.h"
#include "hardware/chemicalinjector.h"
#include "hardware/hardwareunit.h"
class PHManager :  public HardwareUnit
{
    Q_OBJECT
public:
    explicit PHManager(QObject *parent = nullptr);
    virtual void reactToParamChanged(Parameter*, float );
    virtual void reactToSensorsChanged();
    virtual void finish();
    void attachInjector(ChemicalInjector* c);
    void regulate();
    float ph();

signals:



private:
    bool m_activ;
    float m_command;
    AnalogSensor* m_sensor;
    ChemicalInjector* m_phPlus;
    ChemicalInjector* m_phMinus;

};

#endif // PHMANAGER_H
