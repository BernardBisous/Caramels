#include "parametereditor.h"

ParameterEditor::ParameterEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr)
{

    setLayout(new QHBoxLayout);

    layout()->addWidget(m_plot=new ParameterPlot);

    m_editWidget=new QWidget;
    m_editWidget->setLayout(new QVBoxLayout);
    m_editWidget->layout()->addWidget(m_name=new NameLabel);
    m_editWidget->layout()->addWidget(m_desc=new QLabel);


    m_editWidget->layout()->addWidget( m_editor=new ParameterValueEditor);
   connect(m_editor,SIGNAL(changed()),this,SLOT(editSlot()));
   m_editor->setPlot(m_plot);

    QWidget* s=new QWidget;
    s->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    m_editWidget->layout()->addWidget(s);


    layout()->addWidget(m_editWidget);

    m_desc->setWordWrap(true);

    m_editWidget->setFixedWidth(250);
    connect(m_plot,SIGNAL(clicked(QPointF)),this,SLOT(pointClickedSlot(QPointF)));
    connect(m_name,SIGNAL(edited(QString)),this,SLOT(nameChangedSlot(QString)));

}

void ParameterEditor::handle(Parameter *p)
{
    m_client=p;

    if(!p)
        return;

    m_editor->setClient(p);
    m_plot->setParameter(p);
    refresh();
}

void ParameterEditor::setXRange(int t)
{
    m_plot->setXrange(t);
}

void ParameterEditor::refresh()
{
    if(!m_client)
        return;

    m_plot->refresh();
    m_name->load(m_client->name());
    m_desc->setText(m_client->description());


}

void ParameterEditor::edit(int i)
{
    m_plot->refresh();
    m_plot->select(i);
}



void ParameterEditor::nameChangedSlot(QString s)
{
    m_client->setName(s);
}

void ParameterEditor::pointClickedSlot(QPointF p)
{



    edit(p.x());



}
/*
void ParameterEditor::wheelEvent(QWheelEvent *event)
{
    // Get the mouse wheel delta (positive for scrolling up, negative for scrolling down)
    m_editor->move(event->angleDelta().y() / 8);
    update();
}

void ParameterEditor::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"pressed key"<<event->key();
    switch(event->key())
    {
    case Qt::Key_Up     :  m_editor->move(1); break;
    case Qt::Key_Down:  m_editor->move(-1); break;
    case Qt::Key_Left:  m_editor->selectOffset(-1); break;
    case Qt::Key_Right:  m_editor->selectOffset(1); break;
    }


    update();
}
*/
void ParameterEditor::editSlot()
{

    m_plot->refresh();
    emit edited();

}

int ParameterEditor::currentIndex() const
{
    return m_currentIndex;
}

void ParameterEditor::setCurrentIndex(int newCurrentIndex)
{
    m_currentIndex = newCurrentIndex;
    edit(newCurrentIndex);
}

