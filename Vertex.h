#ifndef VERTEX_H
#define VERTEX_H

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QStyleOption>

#include <list>
#include "Edge.h"
#include "GraphArea.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Vertex : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Vertex(GraphArea* graphArea);

    void addEdge(Edge* edge);
    std::list<Edge*> edges() const;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    bool advancePosition();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void changeColor(QColor color);

    void setPosData(QPoint point) { xPos = point.x(); yPos = point.y(); }
    QPoint getPosData() const { return QPoint(xPos, yPos); }

    void setCandidatePairFound(bool b) { candidatePairFound = b; }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int dummy = 5;
    int xPos;
    int yPos;
    std::list<Edge *> edgeList;
    QPointF newPos;
    GraphArea* graphArea;
    QColor vertexColor = Qt::black;

    bool candidatePairFound = false;

signals:
    void vertexClicked(Vertex*);
    void promptCreatePair(Vertex*);
};

#endif // VERTEX_H
