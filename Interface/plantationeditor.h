#ifndef PLANTATIONEDITOR_H
#define PLANTATIONEDITOR_H

#include "plantation.h"
#include <QWidget>

class PlantationEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PlantationEditor(QWidget *parent = nullptr);
    void handle(Plantation*t);


signals:

private:
    Plantation* m_client;

};

#endif // PLANTATIONEDITOR_H
