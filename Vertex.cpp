#include "Vertex.h"

#include <QDebug>

/**
 * @brief Vertex constructor.
 * @param graphArea Passed to QLabel() constructor.
 */

Vertex::Vertex(GraphArea *graphArea)
    : graphArea(graphArea)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

/**
 * @brief Adds edge to edgeList
 * @param edge Edge
 */
void Vertex::addEdge(Edge *edge)
{
    edgeList.push_back(edge);
    edge->adjust();
}

/**
 * @brief Gets a list of edges.
 * @return STL list of Edge*
 */
std::list<Edge*> Vertex::edges() const
{
    return edgeList;
}

bool Vertex::advancePosition()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Vertex::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath Vertex::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Vertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::black);
    painter->drawEllipse(-14, -14, 28, 28);
    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-12, -12, 24, 24);
    painter->drawText(-5, 5, QString::number(dummy));
}

QVariant Vertex::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        for (Edge *edge : qAsConst(edgeList))
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Vertex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    if (event->buttons() == Qt::RightButton) {
//        for (Edge* e : edgeList) {
//            delete e;
//        }
//        delete this;
//        return;
//    }
    emit vertexClicked(this);

    update();
    QGraphicsItem::mousePressEvent(event);
}

void Vertex::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
