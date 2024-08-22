#include "growingtent.h"
GrowingTent::GrowingTent()
    : QMainWindow{}

{
    loadStyle();

    m_editor=new ConfigEditor(this);
    setCentralWidget(m_editor);

    m_currentConfig=new GrowConfig;

    m_tent=new Tent;
    m_tent->setConfig(m_currentConfig);
    m_editor->handle(m_currentConfig);


}

void GrowingTent::loadStyle()
{
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(49, 44, 64));
    darkPalette.setColor(QPalette::WindowText, QColor(215, 213, 218));
    darkPalette.setColor(QPalette::Base, QColor(44, 40, 58));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipText, QColor(215, 213, 218));

    // Text colors
    darkPalette.setColor(QPalette::Text, QColor(215, 213, 218));
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::PlaceholderText, QColor(127, 127, 127));

    // Button colors
    darkPalette.setColor(QPalette::Button, QColor(154, 0, 181));
    darkPalette.setColor(QPalette::ButtonText, QColor(215, 213, 218));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));

    // Highlight colors
    darkPalette.setColor(QPalette::Highlight, QColor(154, 0, 181));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Link, QColor(77, 58, 172));

    // Additional colors (optional)
    darkPalette.setColor(QPalette::Dark, QColor(35, 35, 35));
    darkPalette.setColor(QPalette::Shadow, QColor(20, 20, 20));
    darkPalette.setColor(QPalette::BrightText,
     Qt::red);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));

    setPalette(darkPalette);
}
