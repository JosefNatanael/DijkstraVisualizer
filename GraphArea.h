#ifndef GRAPHAREA_H
#define GRAPHAREA_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

#include <list>
#include <vector>
#include <utility>

#include "Utilities/PriorityQueue.h"
#include "Utilities/UnvisitedVertices.h"

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
        STARTED,
        SHOWPATH,
        VISUALIZED
    };

public:
    explicit GraphArea(QWidget *parent = nullptr);
    ~GraphArea();
    void setCursorMode(Cursor cursor) { this->cursor = cursor; }
    Cursor getCursorMode() const { return this->cursor; }

protected:
    void mousePressEvent(QMouseEvent* event);

private slots:
    void onVertexClicked(Vertex*);
    void onPromptCreatePair(Vertex*);
    void onDestroyVertex(Vertex*);

private:
    void startAlgorithm();
    void clearColoredEdges();

private:
    QGraphicsScene* graphicsScene;
    Cursor cursor = Cursor::POINTER;
    std::vector<Vertex*> adjacencyList;
    std::vector<Edge*> coloredEdges;
    Vertex* startVertex = nullptr;

    PriorityQueue<Vertex*>* minHeap = nullptr;
    UnvisitedVertices* unvisitedVertices = nullptr;
    Vertex* dijkstraSourceVertex = nullptr;
    Vertex* dijkstraCurrentVertex = nullptr;
    Vertex* dijkstraDestinationVertex = nullptr;

    bool isVisualized = false;

signals:
    void turnOffStartButton();      // Slot in MainWindow
    void turnOffShowPathButton();   // Slot in MainWindow
};

#endif // GRAPHAREA_H
