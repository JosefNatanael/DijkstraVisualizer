#ifndef GRAPHAREA_H
#define GRAPHAREA_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

#include <list>

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
    std::list<Vertex*> vertices;
    Vertex* startVertex = nullptr;

signals:
};

#endif // GRAPHAREA_H
