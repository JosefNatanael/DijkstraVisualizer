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
    return QRectF(-20 - adjust, -20 - adjust, 40 + adjust, 40 + adjust);
}

QPainterPath Vertex::shape() const
{
    QPainterPath path;
    path.addEllipse(-18, -18, 36, 36);
    return path;
}

void Vertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(vertexColor);
    painter->drawEllipse(-18, -18, 36, 36);
    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-16, -16, 32, 32);
    painter->drawText(-5, 5, QString::number(dummy));
}

void Vertex::changeColor(QColor color)
{
    vertexColor = color;
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
    if (candidatePairFound) {
        emit promptCreatePair(this);
    }
    candidatePairFound = false;
}
