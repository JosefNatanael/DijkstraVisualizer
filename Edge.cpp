#include <qmath.h>
#include <QPainter>

#include <QDebug>

#include "Edge.h"
#include "Vertex.h"
#include <GraphArea.h>

Edge::Edge(Vertex *sourceNode, Vertex *destNode, GraphArea* graphArea, int weight)
    : graphArea(graphArea)
    , weight(weight)
{
    source = sourceNode;
    destination = destNode;
    source->addPair(destination, this);
    destination->addPair(source, this);

    adjust();
}

Vertex* Edge::getSourceVertex() const
{
    return source;
}

Vertex* Edge::getDestinationVertex() const
{
    return destination;
}

void Edge::detachFromIncidentVertices()
{
    source->removePair(destination, this);
    destination->removePair(source, this);
    source = destination = nullptr;
}

void Edge::adjust()
{
    if (!source || !destination)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(destination, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destinationPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destinationPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const
{
    if (!source || !destination)
        return QRectF();

    qreal extra = 15;

    return QRectF(sourcePoint, QSizeF(destinationPoint.x() - sourcePoint.x(),
                                      destinationPoint.y() - sourcePoint.y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Edge::shape() const
{
    static const qreal tolerance = 3;
    QPointF startP = sourcePoint;
    QPointF endP = destinationPoint;
    QPointF vec = endP - startP;
    vec = vec * (tolerance / qSqrt(QPointF::dotProduct(vec, vec)));
    QPointF orthogonal(vec.y(), -vec.x());

    QPainterPath result(startP - vec + orthogonal);
    result.lineTo(startP - vec - orthogonal);
    result.lineTo(endP + vec - orthogonal);
    result.lineTo(endP + vec + orthogonal);
    result.closeSubpath();

    return result;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !destination)
        return;

    QLineF line(sourcePoint, destinationPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(lineColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw weight text
    QPointF vec = sourcePoint - destinationPoint;
    QPointF orthogonal(vec.y(), -vec.x());
    qreal length = qSqrt(QPointF::dotProduct(vec, vec));

    QPointF unitOrthogonal(orthogonal.x() / length, orthogonal.y() / length);
    QPointF middlePos = (sourcePoint + destinationPoint) / 2;
    QPointF pos = middlePos + 10 * unitOrthogonal;

//    QPointF pos = (sourcePoint + destinationPoint) / 2;
    painter->drawText(pos, QString::number(weight));
}

void Edge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton && graphArea->getCursorMode() == GraphArea::Cursor::POINTER) {
        detachFromIncidentVertices();
        delete this;
        return;
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Edge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
