#include "roundedpicture.h"
#include "qpainter.h"
#include "qpainterpath.h"

RoundedPicture::RoundedPicture(int radius, QWidget *parent)
    : QLabel{parent},m_radius(radius)
{

}



void RoundedPicture::paintEvent(QPaintEvent *)
{
    //QLabel::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing, true);

    QRect rect = this->rect();
    QPixmap pixmap = this->pixmap();
    pixmap = pixmap.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int cornerRadius = m_radius;
    QPainterPath path;
    path.addRoundedRect(rect, cornerRadius, cornerRadius);
    painter.setClipPath(path);
    painter.drawPixmap(QPoint(0, 0), pixmap);
}

int RoundedPicture::radius() const
{
    return m_radius;
}

void RoundedPicture::setRadius(int newRadius)
{
    m_radius = newRadius;
}
