#ifndef REGULATORWIDGET_H
#define REGULATORWIDGET_H

#include <QWidget>
#include "Interface/toolbutton.h"
#include "hardware/analogsensor.h"
#include "qlabel.h"
class RegulatorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegulatorWidget(QWidget *parent = nullptr);
    void handle(AnalogSensor* a);

public slots:
    void refresh();

signals:

private slots:
    void valueSlot(float t);
    void buttonSlot();


private:
    AnalogSensor*m_client;
    QLabel* m_regLabel;
    QLabel* m_valueLabel;
    ToolButton* m_button;

};

#endif // REGULATORWIDGET_H
