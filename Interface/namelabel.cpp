#include "namelabel.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include "qlineedit.h"

NameLabel::NameLabel(QWidget *parent)
    : QWidget{parent}
{
    m_label = new QLabel;
    m_edit = new QLineEdit;
    m_edit->hide();

    m_label->installEventFilter(this);
    setLayout(new QVBoxLayout);
    layout()->addWidget(m_label);
    layout()->addWidget(m_edit);
    layout()->setContentsMargins(0,0,0,0);




    m_edit->setPlaceholderText("Enter name");

    connect(m_edit, &QLineEdit::editingFinished, this, &NameLabel::finishedSlot);

    setTextSize(16);
}

void NameLabel::setTextSize(int pointSize)
{
    QFont f=font();
    f.setPointSize(pointSize);
    f.setBold(true);
    m_label->setFont(f);
    m_edit->setFont(f);
    setFixedHeight(pointSize+10);
}

void NameLabel::load(QString s)
{
    m_label->setText(s);
    m_edit->setText(s);
}



void NameLabel::setEditMode(bool s)
{
    m_edit->setHidden(!s);
    m_label->setHidden(s);
}

void NameLabel::clickedSlot()
{

    setEditMode(true);
}

void NameLabel::finishedSlot()
{
    emit edited(m_edit->text());

    setEditMode(false);
}


