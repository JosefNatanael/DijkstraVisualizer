#ifndef GRAPHAREA_H
#define GRAPHAREA_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

#include <list>
#include <vector>
#include <utility>

class Vertex;

class GraphArea : public QGraphicsView
{
    Q_OBJECT
public:
    enum class Cursor {
        POINTER,
        VERTEX,
        EDGE,
        START,
        SHOWPATH,
        VISUALIZED
    };

public:
    explicit GraphArea(QWidget *parent = nullptr);
    ~GraphArea();
    void setCursorMode(Cursor cursor);
    Cursor getCursorMode() const;

protected:
    void mousePressEvent(QMouseEvent* event);

private slots:
    void onVertexClicked(Vertex*);
    void onPromptCreatePair(Vertex*);

private:
    QGraphicsScene* graphicsScene;
    Cursor cursor = Cursor::POINTER;
    std::vector<Vertex*> vertices;
    std::vector<std::list<Vertex*>> adjacencyList;
    Vertex* startVertex = nullptr;

signals:
};

/**
 * @brief Sets cursor.
 * @param cursor
 */
void GraphArea::setCursorMode(Cursor cursor)
{
    this->cursor = cursor;
}

GraphArea::Cursor GraphArea::getCursorMode() const
{
    return this->cursor;
}

#endif // GRAPHAREA_H
