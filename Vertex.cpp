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
 * @brief Removes edge from edgeList
 * @param edge Edge
 */
void Vertex::removeEdge(Edge *edge)
{
    std::list<Edge*>::iterator it;
    it = std::find(edgeList.begin(), edgeList.end(), edge);
    if (it != edgeList.end()) {
        edgeList.erase(it);
    }
}

/**
 * @brief Gets a list of edges.
 * @return STL list of Edge*
 */
std::list<Edge*> Vertex::edges() const
{
    return edgeList;
}

/**
 * @brief Adds vertex to verticesList.
 * @param vertex
 */
void Vertex::addVertex(Vertex *vertex)
{
    vertexList.push_back(vertex);
}

/**
 * @brief Remove vertex from vertexList
 * @param vertex
 */
void Vertex::removeVertex(Vertex *vertex)
{
    std::list<Vertex*>::iterator it;
    it = std::find(vertexList.begin(), vertexList.end(), vertex);
    if (it != vertexList.end()) {
        vertexList.erase(it);
    }
}

/**
 * @brief Gets a list of vertices.
 * @return STL list of Vertex*
 */
std::list<Vertex *> Vertex::vertices()
{
    return vertexList;
}

/**
 * @brief Make pair and add to vertexEdgeList.
 * @param vertex
 * @param edge
 */
void Vertex::addPair(Vertex *vertex, Edge *edge)
{
    vertexEdgeList.push_back(std::make_pair(vertex, edge));
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
        vertexEdgeList.erase(it);
    }
}

/**
 * @brief Gets a list of pairs.
 * @return STL list of Vertex* Edge* pairs
 */
list<pair<Vertex *, Edge *> > Vertex::pairs()
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
    if (event->buttons() == Qt::LeftButton) {
        emit vertexClicked(this);
    }
    else if (event->buttons() == Qt::RightButton && graphArea->getCursorMode() == GraphArea::Cursor::POINTER) {
        for (Edge* e : edgeList) {
            e->detachFromIncidentVertices();
            delete e;
        }
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
