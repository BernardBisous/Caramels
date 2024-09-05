#ifndef CHEMICALINJECTOR_H
#define CHEMICALINJECTOR_H

#include "hardware/booleansensor.h"
#include "hardware/pump.h"
#include "parameter.h"
#include <QObject>

class ChemicalInjector :public QObject
{

    Q_OBJECT
public:
    explicit ChemicalInjector(int mixPin, int pumpPin, int LevelPin ,int ID, QObject*p=nullptr);

    void setPump(ChemicalPump *newPump);

    void injectMl(float v);
    int msForMl(float v);
    void setGain( float msPerMl);
    float gain();

    QDateTime lastInjection() const;

    void setMaxMsInjection(int newMaxMsInjection);

    SwitchedActuator *mixer() const;
    ChemicalPump *pump() const;
    BooleanSensor *levelSensor() const;



    int id() const;

    bool injecting() const;

signals:
    void injection(int purcent);
    void console(QString s);

private slots:

    void mixerSlot();
    void pumpSlot();



private:
    bool m_injecting;
    int m_maxMsInjection;
    QDateTime m_lastInjection;
    ChemicalPump* m_pump;
    SwitchedActuator* m_mixer;
    BooleanSensor* m_levelSensor;
    int m_id;
    float m_injectingValue;





};

#endif // CHEMICALINJECTOR_H
