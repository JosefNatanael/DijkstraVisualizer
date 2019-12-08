#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

#include "GraphArea.h"

class Vertex;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Edge : public QGraphicsItem
{
public:
    explicit Edge(Vertex* sourceVertex, Vertex* destVertex, GraphArea* graphArea, int weight);

    Vertex* getSourceVertex() const;
    Vertex* getDestinationVertex() const;
    int getWeight() const { return weight; }
    void setLineColor(QColor c) { lineColor = c; }
    void detachFromIncidentVertices();

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Vertex* source;
    Vertex* destination;
    GraphArea* graphArea;
    int weight = 0;

    QPointF sourcePoint;
    QPointF destinationPoint;
    QColor lineColor = Qt::black;
};

#endif // EDGE_H
