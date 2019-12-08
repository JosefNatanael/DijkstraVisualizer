#include "GraphArea.h"
#include "Vertex.h"
#include "Edge.h"

#include <QDebug>
#include "Utilities/WindowHelper.h"
#include <QDesktopWidget>
#include <QInputDialog>

#include <limits>

/**
 * @brief GraphArea constructor.
 * @param parent Passed to QGraphicsView() constructor.
 */
GraphArea::GraphArea(QWidget *parent)
    : QGraphicsView(parent)
    , graphicsScene(new QGraphicsScene(this))
{
    setScene(graphicsScene);
//    scene()->installEventFilter(this);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setFocusPolicy(Qt::NoFocus);
    QRect screenRect = WindowHelper::screenFromWidget(qApp->desktop())->geometry();
    setSceneRect(0, 0, screenRect.width(), screenRect.height());
}

/**
 * @brief GraphArea destructor.
 * @deletes all vertices.
 */
GraphArea::~GraphArea()
{
    for (Vertex* v : adjacencyList) {
        delete v;
    }
    delete unvisitedVertices;
    delete minHeap;
}

/**
 * @brief Detects mousePressEvent, specifically position.
 * @details Emits clicked position in the visualizer area.
 * @param event Mouse press event.
 */
void GraphArea::mousePressEvent(QMouseEvent* event)
{
    // Pass to all else as well.
    QGraphicsView::mousePressEvent(event);
    QGraphicsView::mousePressEvent(event);

    // DEBUGGING PURPOSES
    qDebug() << static_cast<int>(cursor);

    // Not left click, return.
    if (event->buttons() != Qt::LeftButton) {
        return;
    }
    switch (cursor) {
    case Cursor::POINTER:
    {
        break;
    }
    case Cursor::VERTEX:
    {
        // If there is a startVertex: set color to black.
        if (startVertex != nullptr) {
            startVertex->changeColor(Qt::black);
            startVertex->update();
        }
        // Create new vertex, add to scene, setup connection, add to vertex list.
        Vertex* vertex = new Vertex(this);
        graphicsScene->addItem(vertex);
        vertex->setPos(event->pos());
        connect(vertex, &Vertex::vertexClicked, this, &GraphArea::onVertexClicked);
        connect(vertex, &Vertex::promptCreatePair, this, &GraphArea::onPromptCreatePair);
        connect(vertex, &Vertex::destroyVertex, this, &GraphArea::onDestroyVertex);
        adjacencyList.push_back(vertex);

        // Sets startVertex to nullptr in case user cancels edge operation.
        startVertex = nullptr;
        break;
    }
    case Cursor::EDGE:
        break;
    case Cursor::START:
        startVertex = nullptr;
        break;
    case Cursor::STARTED:
        break;
    case Cursor::SHOWPATH:
    {
        startVertex = nullptr;
        break;
    }
    case Cursor::VISUALIZED:
        break;
    }
}

void GraphArea::onVertexClicked(Vertex* vertex)
{
    if (vertex == startVertex) {
        return;
    }
    if (cursor == Cursor::EDGE && startVertex == nullptr) {
        startVertex = vertex;
        startVertex->changeColor(QColor(200, 200, 0));
        startVertex->update();
    }
    else if (cursor == Cursor::EDGE && startVertex != nullptr) {
        // Basically tells the vertex to emit a signal that calls onPromptCreatePair, via mouse release.
        vertex->setCandidatePairFound(true);
    }
    else if (cursor == Cursor::START) {
        cursor = Cursor::STARTED;
        dijkstraSourceVertex = dijkstraCurrentVertex = vertex;
        vertex->changeColor(QColor(26, 83, 255));
        vertex->setDistance(0);
        emit turnOffStartButton();
    }
    else if (cursor == Cursor::STARTED) {
        emit turnOffStartButton();
        startAlgorithm();
    }
    else if (cursor == Cursor::VISUALIZED) {
        emit turnOffStartButton();
    }
    else if (cursor == Cursor::SHOWPATH) {
        if (!isVisualized) {
            emit turnOffShowPathButton();
            return;
        }
        // Show path algorithm
        clearColoredEdges();
        Vertex* before = vertex->getPreviousVertex();
        Vertex* current = vertex;
        while (before != nullptr) {
            list<pair<Vertex*, Edge*>>& currentList = current->pairs();
            list<pair<Vertex*, Edge*>>::iterator it;
            it = std::find_if(currentList.begin(), currentList.end(), [before](pair<Vertex*, Edge*> p){ return p.first == before; });
            if (it != currentList.end()) {
                it->second->setLineColor(QColor(26, 83, 255));
                it->second->update();
                coloredEdges.push_back(it->second);
            }
            else {
                qDebug() << "This is impossible!!";
            }
            current = current->getPreviousVertex();
            before = before->getPreviousVertex();
        }
    }
}

void GraphArea::onPromptCreatePair(Vertex* pairVertex)
{
    startVertex->changeColor(Qt::black);
    startVertex->update();
    bool ok;
    int weight = QInputDialog::getInt(this, "Input Weight", "Input Weight", 0, 0, std::numeric_limits<int>::max(), 1, &ok);
    if (ok) {
        graphicsScene->addItem(new Edge(startVertex, pairVertex, this, weight));
    }
    startVertex = nullptr;
}

void GraphArea::onDestroyVertex(Vertex* vertex)
{
    std::vector<Vertex*>::iterator it = std::find(adjacencyList.begin(), adjacencyList.end(), vertex);
    if (it != adjacencyList.end()) {
        it = adjacencyList.erase(it);
    }
    else {
        qDebug() << "not supposed to happen!";
    }
}

void GraphArea::startAlgorithm()
{
    minHeap = new PriorityQueue<Vertex*>;
    // Create a set (hashtable) of all unvisited vertices.
    unvisitedVertices = new UnvisitedVertices(static_cast<int>(adjacencyList.size()));
    // Mark all vertices as unvisited.
    for (unsigned int i = 0; i < adjacencyList.size(); ++i) {
        adjacencyList[i]->setID(static_cast<int>(i));
        unvisitedVertices->insertNode(static_cast<int>(i), adjacencyList[i]);
    }
    // Insert the source vertex into the priority queue.
    minHeap->insert(dijkstraSourceVertex);
    while (!minHeap->isEmpty()) {
        Vertex* minDist = minHeap->findMin();
        if (minDist != nullptr) {
            if (unvisitedVertices->exists(minDist->getID())) {
                dijkstraCurrentVertex = minDist;
                unvisitedVertices->removeNode(minDist->getID());
            }
            minDist->changeColor(Qt::gray);
            minHeap->remove(minDist);
        }
        list<pair<Vertex*, Edge*>>& currentPairsList = dijkstraCurrentVertex->pairs();
        for (list<pair<Vertex*, Edge*>>::iterator it = currentPairsList.begin(); it != currentPairsList.end(); ++it) {
            if (!unvisitedVertices->exists(it->first->getID()))
                continue;
            int currentNeighborDistance = it->first->getDistance();
            int potentialNewDist = it->second->getWeight() + dijkstraCurrentVertex->getDistance();
            if (potentialNewDist < currentNeighborDistance) {
                it->first->setDistance(potentialNewDist);
                it->first->setPreviousVertex(dijkstraCurrentVertex);
                if (!it->first->getWasInPriorityQueue()) {
                    minHeap->insert(it->first);
                    it->first->setWasInPriorityQueue(true);
                }

            }

        }
    }
    cursor = Cursor::VISUALIZED;
    isVisualized = true;
}

void GraphArea::clearColoredEdges()
{
    for (Edge* e : coloredEdges) {
        e->setLineColor(Qt::black);
        e->update();
    }
    coloredEdges.clear();
}
