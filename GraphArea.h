#ifndef GRAPHAREA_H
#define GRAPHAREA_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

#include <list>
#include <vector>
#include <utility>

#include "Utilities/PriorityQueue.h"
#include "Utilities/RedBlackPriority.h"
#include "Utilities/UnvisitedVertices.h"

class Vertex;

class GraphArea : public QGraphicsView
{
    Q_OBJECT
    friend class MainWindow;
public:
    enum class Cursor {
        POINTER,
        VERTEX,
        EDGE,
        START,
        STARTED,
        SHOWPATH,
        CALCULATED
    };

public:
    explicit GraphArea(QWidget *parent = nullptr);
    ~GraphArea();
    void setCursorMode(Cursor cursor) { this->cursor = cursor; }
    Cursor getCursorMode() const { return this->cursor; }

    void randomGenerator();

protected:
    void mousePressEvent(QMouseEvent* event);

private slots:
    void onVertexClicked(Vertex*);
    void onPromptCreatePair(Vertex*);
    void onDestroyVertex(Vertex*);

private:
    void startAlgorithm();
    template <typename T> void coreAlgorithm(T);
    void clearColoredEdges();
    void clearAlgorithm();
    void newSlate();
    void removeAndDeallocVertexEdge();

private:
    enum class Command {
        NONE,
        CURRENTVERTEX,
        VERTEXUPDATE,
        VERTEXNOUPDATE,
        VISITEDVERTEX,
        UPDATEDINHEAP,
        CLEARALLVERTEX
    };
    struct Action {
        Command commandType = Command::NONE;
        Vertex* vertexConsidered = nullptr;
        Vertex* previousVertex = nullptr;
        Edge* edgeConsidered = nullptr;
        int distance = -99;
        Action() = delete;
        explicit Action(Command cmd, Vertex* vertex = nullptr, Vertex* preVer = nullptr, Edge* edge = nullptr, int dist = -99)
            : commandType(cmd)
            , vertexConsidered(vertex)
            , previousVertex(preVer)
            , edgeConsidered(edge)
            , distance(dist)
        {}
    };

private:
    QGraphicsScene* graphicsScene;
    Cursor cursor = Cursor::POINTER;
    std::vector<Vertex*> adjacencyList;
    std::vector<Edge*> coloredEdges;
    Vertex* startVertex = nullptr;

    PriorityQueue<Vertex*>* avlPriority = nullptr;
    RedBlackPriority* rbPriority = nullptr;
    UnvisitedVertices* unvisitedVertices = nullptr;
    Vertex* dijkstraSourceVertex = nullptr;
    Vertex* dijkstraCurrentVertex = nullptr;
    Vertex* dijkstraDestinationVertex = nullptr;

    std::list<Action> actionsList;

    bool isCalculated = false;
    bool isVisualized = false;

    bool useAVL = true;     // False: use Red Black Tree

signals:
    void turnOffStartButton();      // Slot in MainWindow
    void turnOffShowPathButton();   // Slot in MainWindow
};

#include "CoreAlgo.tpp"

#endif // GRAPHAREA_H
