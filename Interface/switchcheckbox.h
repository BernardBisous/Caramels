#ifndef SWITCHCHECKBOX_H
#define SWITCHCHECKBOX_H


#include<QCheckBox>
class SwitchCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit SwitchCheckBox(QString s="",QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;





signals:

private:






};

#endif // SWITCHCHECKBOX_H
