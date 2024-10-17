#ifndef ARCHIVEWIDGET_H
#define ARCHIVEWIDGET_H

#include "Interface/actionwidget.h"
#include "Interface/scrollarea.h"
#include "Interface/toolbutton.h"
#include "config/archive.h"
#include "qformlayout.h"
#include "qlabel.h"
#include <QWidget>




class ArchiveItem: public ActionWidget
{
    Q_OBJECT
public:
    explicit ArchiveItem(Archive c, QWidget *parent = nullptr);


signals:
    void reuse();
private slots:
    void reuseSlot();
private:
    QLabel* m_name;
    QLabel* m_desc;
    QLabel* m_pix;
    QLabel* m_date;
    QLabel* m_plants;
    QLabel* m_result;
    QLabel* m_supply;
    QFormLayout* m_form;
    ToolButton*  m_reuseButton;
};

class ArchiveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ArchiveWidget(QWidget *parent = nullptr);
    void load();
    void loadArchive(QString s);


signals:

private:
    ScrollArea* m_list;

};

#endif // ARCHIVEWIDGET_H
