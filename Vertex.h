#ifndef VERTEX_H
#define VERTEX_H

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsObject>

#include <list>
#include <algorithm>
#include <utility>

#include "Edge.h"
#include "GraphArea.h"

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

    void                            addEdge(Edge* edge);
    void                            removeEdge(Edge* edge);
    std::list<Edge*>                edges() const;
    void                            addVertex(Vertex* vertex);
    void                            removeVertex(Vertex* vertex);
    std::list<Vertex*>              vertices();
    void                            addPair(Vertex* vertex, Edge* edge);
    void                            removePair(Vertex* vertex, Edge* edge);
    list<pair<Vertex*, Edge*>>      pairs();

    enum { Type = UserType + 1 };
    int                             type() const override { return Type; }
    bool                            advancePosition();

    // Draw Vertex
    QRectF                          boundingRect() const override;
    QPainterPath                    shape() const override;
    void                            paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void                            changeColor(QColor color);

    void                            setCandidatePairFound(bool b) { candidatePairFound = b; }

protected:
    QVariant                        itemChange(GraphicsItemChange change, const QVariant &value) override;
    void                            mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void                            mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int                             distance = 0;
    list<Edge*>                     edgeList;
    list<Vertex*>                   vertexList;
    list<pair<Vertex*, Edge*>>      vertexEdgeList;
    QPointF                         newPos;
    GraphArea*                      graphArea;
    QColor                          vertexColor = Qt::black;

    bool                            candidatePairFound = false;

signals:
    void vertexClicked(Vertex*);        // Slot at GraphArea
    void promptCreatePair(Vertex*);     // Slot at GraphArea
};

#endif // VERTEX_H
