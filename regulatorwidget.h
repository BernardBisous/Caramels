#ifndef REGULATORWIDGET_H
#define REGULATORWIDGET_H

#include <QWidget>
#include "Interface/toolbutton.h"
#include "hardware/analogsensor.h"
#include "hardware/regulatingtimer.h"
#include "qlabel.h"
class RegulatorWidget : public ActionWidget
{
    Q_OBJECT
public:
    explicit RegulatorWidget(QWidget *parent = nullptr);
    void handle(AnalogSensor* a);


    void reset();
    RegulatingTimer *regulator() const;
    void setRegulator(RegulatingTimer *newRegulator);

public slots:
    void refresh();

signals:

private slots:
    void valueSlot(float t);
    void buttonSlot();


private:
    RegulatingTimer* m_regulator;
    AnalogSensor*m_client;
    QLabel* m_regLabel;
    QLabel* m_valueLabel;
    ToolButton* m_button;
    bool m_startsHigh;

};

#endif // REGULATORWIDGET_H
