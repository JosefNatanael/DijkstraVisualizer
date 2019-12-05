#include <qmath.h>
#include <QPainter>

#include "Edge.h"
#include "Vertex.h"

Edge::Edge(Vertex *sourceNode, Vertex *destNode, int weight) : weight(weight)
{
    setAcceptedMouseButtons(nullptr);
    source = sourceNode;
    destination = destNode;
    source->addEdge(this);
    destination->addEdge(this);
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

    qreal penWidth = 1;
    qreal extra = (penWidth + edgeLength) / 2.0;

    return QRectF(sourcePoint, QSizeF(destinationPoint.x() - sourcePoint.x(),
                                      destinationPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !destination)
        return;

    QLineF line(sourcePoint, destinationPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw weight text
    QPointF pos = (sourcePoint + destinationPoint) / 2;
    painter->drawText(pos, QString::number(weight));
}
