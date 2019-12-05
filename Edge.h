#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Vertex;

class Edge : public QGraphicsItem
{
public:
    explicit Edge(Vertex* sourceVertex, Vertex* destVertex, int weight);

    Vertex* getSourceVertex() const;
    Vertex* getDestinationVertex() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Vertex* source;
    Vertex* destination;

    int weight = 0;

    QPointF sourcePoint;
    QPointF destinationPoint;
    qreal edgeLength = 10;
};

#endif // EDGE_H
