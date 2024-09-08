#ifndef PARAMETERVALUEEDITOR_H
#define PARAMETERVALUEEDITOR_H

#include "Interface/toolbutton.h"
#include "parameter.h"
#include "qlabel.h"
#include "qlineseries.h"
#include <QWidget>

class ParameterValueEditor : public QWidget
{
    Q_OBJECT
public:
    typedef enum mode{modeGain=0,modeOffset,modeAlone,modeRemove}Mode;
    explicit ParameterValueEditor(QWidget *parent = nullptr);

    Parameter *client() const;
    void setClient(Parameter *newClient);


    void refresh();
    ToolButton *addMode(QString icon,QString title);
    Mode mode() const;
    void setMode(Mode newMode);
    void refreshMode();
    void move(int inc);
    void edit(QList<int>indexes );
    void edit(int i);

    void setGain(float newGain);

    void setOffset(float newOffset);
    void selectOffset(int v);

    void setSeries(QLineSeries *newSeries);
    void refreshValue();
    int lastSelected();

signals:
    void changed();



public slots:
    void modeEdited();
    void seriesSelected();

    void up();
    void down();
    void left();
    void right();

private:
    Parameter* m_client;
    QList<ToolButton*> m_modes;
    Mode m_mode;


    ToolButton* m_up;
    ToolButton* m_down;
    ToolButton* m_left;
    ToolButton* m_right;

    QList<int>m_indexes;
    QLabel* m_value;



    QLineSeries* m_series;

    float m_gain;

};

#endif // PARAMETERVALUEEDITOR_H
