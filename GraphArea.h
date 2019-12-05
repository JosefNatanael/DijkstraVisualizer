#ifndef GRAPHAREA_H
#define GRAPHAREA_H

#include <QGraphicsView>
#include <QMouseEvent>

class GraphArea : public QGraphicsView
{
    Q_OBJECT
public:
    enum class Cursor {
        POINTER,
        VERTEX,
        EDGE,
        START,
        SHOWPATH
    };

public:
    explicit GraphArea(QWidget *parent = nullptr);
    void setCursorMode(Cursor cursor);

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    Cursor cursor = Cursor::POINTER;

signals:
    void drawVertexClicked(QPoint);
    void drawEdgeClicked(QPoint);

};

#endif // GRAPHAREA_H
