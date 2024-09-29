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

    typedef enum state{ready=0,mixing,injecting,empty}State;
    explicit ChemicalInjector(int mixPin, int pumpPin, int LevelPin ,int ID, QObject*p=nullptr);




    void setPump(Pump *newPump);

    void injectMl(float v);


    void setGain( float msPerMl);
    float gain();

    bool isLow();

    void setName(QString s);
    SwitchedActuator *mixer() const;
    Pump *pump() const;
    BooleanSensor *levelSensor() const;

    int id() const;
    State state() const;


    void enable(bool s);
    bool isEnabled();
    float remainingPurcent();
    float totalInjected();

    QString name() const;

    QList<RealTimeValue> injectionHistoric();

    void setVolumeMl(float newVolumeMl);
    void setFlow(float f);

signals:
    void injection(int purcent);
    void console(QString s);
    void stateChanged(ChemicalInjector::State s);

private slots:
    void mixerSlot();
    void pumpSlot();

private:
    void setState(State newState);
    Pump* m_pump;
    SwitchedActuator* m_mixer;
    BooleanSensor* m_levelSensor;
    State m_state;
    int m_id;
    float m_injectingValue;
    bool m_enable;
    QString m_name;
    float m_volumeMl;
};

#endif // CHEMICALINJECTOR_H
