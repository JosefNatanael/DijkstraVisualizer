#include "Vertex.h"

#include <vector>
#include <QDebug>

#include "Edge.h"

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
 * @brief Make pair and add to vertexEdgeList.
 * @param vertex
 * @param edge
 */
void Vertex::addPair(Vertex *vertex, Edge *edge)
{
    vertexEdgeList.push_back(std::make_pair(vertex, edge));
    edge->adjust();
}

/**
 * @brief Removes pair from vertexEdgeList
 * @param vertex
 * @param edge
 */
void Vertex::removePair(Vertex *vertex, Edge *edge)
{
    list<pair<Vertex*, Edge*>>::iterator it;
    it = std::find(vertexEdgeList.begin(), vertexEdgeList.end(), std::make_pair(vertex, edge));
    if (it != vertexEdgeList.end()) {
        it = vertexEdgeList.erase(it);
    }
}

/**
 * @brief Gets a list of pairs.
 * @return STL list of Vertex* Edge* pairs
 */
list<pair<Vertex *, Edge *> >& Vertex::pairs()
{
    return vertexEdgeList;
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
    painter->setPen(vertexColor);
    painter->setBrush(vertexColor);
    painter->drawEllipse(-18, -18, 36, 36);
    painter->setBrush(Qt::white);
    painter->drawEllipse(-16, -16, 32, 32);
    if (distanceWhenVis != std::numeric_limits<int>::max()) {
//        painter->drawText(-5, 5, QString::number(distance));
        painter->drawText(-5, 5, QString::number(distanceWhenVis));
    }
}

QVariant Vertex::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        for (pair<Vertex*, Edge*> e : qAsConst(vertexEdgeList))
            e.second->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Vertex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        setFlag(ItemIsMovable, true);
        if (graphArea->getCursorMode() == GraphArea::Cursor::EDGE) {
            setFlag(ItemIsSelectable);
            setFlag(ItemIsMovable, false);
        }
        emit vertexClicked(this);
    }
    else if ((event->buttons() == Qt::RightButton && graphArea->getCursorMode() == GraphArea::Cursor::POINTER)
             || graphArea->getCursorMode() == GraphArea::Cursor::START)
    {
        std::vector<list<pair<Vertex*, Edge*>>::iterator> iteratorsToRemove;
        for (list<pair<Vertex*, Edge*>>::iterator it = vertexEdgeList.begin(); it != vertexEdgeList.end(); ++it) {
            iteratorsToRemove.push_back(it);
        }
        while (!iteratorsToRemove.empty()) {
            Edge* temp = iteratorsToRemove[iteratorsToRemove.size() - 1]->second;
            iteratorsToRemove[iteratorsToRemove.size() - 1]->second->detachFromIncidentVertices();
            delete temp;
            iteratorsToRemove.pop_back();
        }
        emit destroyVertex(this);
        delete this;
        return;
    }
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
