#ifndef VERTEX_H
#define VERTEX_H

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsObject>

#include <limits>
#include <list>
#include <algorithm>
#include <utility>

class Edge;
class GraphArea;

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

using std::pair;
using std::list;

class Vertex : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Vertex(GraphArea* graphArea);

    void                            addPair(Vertex* vertex, Edge* edge);
    void                            removePair(Vertex* vertex, Edge* edge);
    list<pair<Vertex*, Edge*>>&     pairs();

    enum { Type = UserType + 1 };
    int                             type() const override { return Type; }
    bool                            advancePosition();

    // Draw Vertex
    QRectF                          boundingRect() const override;
    QPainterPath                    shape() const override;
    void                            paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void                            changeColor(QColor color) { vertexColor = color; }

    void                            setCandidatePairFound(bool b) { candidatePairFound = b; }
    int                             getDistance() const { return distance; }
    void                            setDistance(int dist) { distance = dist; update(); }
    int                             getDistanceWhenVis() const { return distanceWhenVis; }
    void                            setDistanceWhenVis(int dist) { distanceWhenVis = dist; update(); }
    int                             getID() const { return uniqueID; }
    void                            setID(int id) { uniqueID = id; }
    bool                            getInPriorityQueue() { return inPriorityQueue; }
    void                            setInPriorityQueue(bool b) { inPriorityQueue = b; }
    void                            setPreviousVertex(Vertex* v) { previousVertex = v; }
    Vertex*                         getPreviousVertex() const { return previousVertex; }
    QColor                          getVertexColor() { return vertexColor; }

protected:
    QVariant                        itemChange(GraphicsItemChange change, const QVariant &value) override;
    void                            mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void                            mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int                             uniqueID = -1;
    int                             distance = std::numeric_limits<int>::max();
    int                             distanceWhenVis = std::numeric_limits<int>::max();
    Vertex*                         previousVertex = nullptr;
    list<pair<Vertex*, Edge*>>      vertexEdgeList;
    bool                            inPriorityQueue = false;

    QPointF                         newPos;
    GraphArea*                      graphArea;
    QColor                          vertexColor = Qt::black;

    bool                            candidatePairFound = false;

signals:
    void vertexClicked(Vertex*);        // Slot at GraphArea
    void promptCreatePair(Vertex*);     // Slot at GraphArea
    void destroyVertex(Vertex*);        // Slot at GraphArea
};

#endif // VERTEX_H
