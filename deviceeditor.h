#ifndef DEVICEEDITOR_H
#define DEVICEEDITOR_H

#include "Interface/actionwidget.h"
#include "Interface/slider.h"
#include "hardware/device.h"
#include "qlabel.h"
#include "qlineedit.h"
#include <QWidget>
#include <QFormLayout>
#include <QGroupBox>

class DeviceEditor : public ActionWidget
{
    Q_OBJECT
public:
    explicit DeviceEditor(QWidget *parent = nullptr);
    void handle(Device* c);
    virtual QBrush backgroundBrush();
    void refresh();
    void setAbstracted(bool s);

    void clearField(QFormLayout*);


    Device *client() const;

signals:
    void edited(QString key,QString value);

private slots:
    void editSlot();
    void sliderEdited(int i);
    void valueSlot(float t);

private:
    QLabel* m_nameLabel;
    QLabel* m_status;
    Device* m_client;
    QFormLayout* m_dataLAyout;
    QFormLayout* m_resultLayout;
    Slider* m_valueSlider;
    QWidget*m_dataWidget;
    QWidget*m_resultWidget;



    bool m_abstracted;

    QList<QLineEdit*>m_dataFields;
   // QWidget*m_resultsWidget;
  //  QFormLayout* m_resultsLayout;


};

#endif // DEVICEEDITOR_H
