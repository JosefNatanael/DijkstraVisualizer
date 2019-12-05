#include "Vertex.h"

/**
 * @brief Vertex::Vertex constructor.
 * @param parent Passed to QLabel() constructor.
 * @param xPos Position of vertex in x direction (relative to parent).
 * @param yPos Position of vertex in y direction (relative to parent).
 */
Vertex::Vertex(int xPos, int yPos, QWidget *parent)
    : QLabel(parent)
    , xPos(xPos)
    , yPos(yPos)
{
}

/**
 * @brief Sets position of the vertex.
 * @point Position.
 */
void Vertex::setPos(QPoint point)
{
    xPos = point.x();
    yPos = point.y();
}

/**
 * @brief Get position of the vertex.
 * @return QPoint
 */
QPoint Vertex::getPos() const
{
    return QPoint(xPos, yPos);
}

/**
 * @brief Overriden protected function. To draw vertex.
 * @param event
 */
void Vertex::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawEllipse(0, 0, width() - 1, height() - 1);
    p.end();
    QLabel::paintEvent(event);
    setText(QString::number(dummy));
}
