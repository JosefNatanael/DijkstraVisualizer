#include "GraphArea.h"
#include "Vertex.h"
#include "Edge.h"
#include "Utilities/RandomGenDialog.h"
#include "Utilities/WindowHelper.h"

#include <QDesktopWidget>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QDateTime>
#include <QtMath>
#include <QDebug>

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
 */
GraphArea::~GraphArea()
{
    removeAndDeallocVertexEdge();   // Redundant call, Vertex and Edge are parented to this and mainwindow, it will auto deallocate.
    delete unvisitedVertices;
    delete minHeap;
}

void GraphArea::randomGenerator()
{
    RandomGenDialog dialog;
    int numVertices = 0;
    double density = 0;
    int maxWeight = 0;
    dialog.setModal(true);
    if (dialog.exec() == QDialog::Accepted) {
        dialog.getValues(numVertices, density, maxWeight);
    }
    else {
        qDebug() << "Rejected";
        return;
    }
    if (numVertices == 0 || qFuzzyCompare(density, 0)) {
        return;
    }
    for (int i = 0; i < numVertices; ++i) {
        QRandomGenerator rand(QRandomGenerator::global()->generate());
        QPoint position(rand.bounded(25, this->size().width() - 25), rand.bounded(25, this->size().height() - 25));
        // Create new vertex, add to scene, setup connection, add to vertex list.
        Vertex* vertex = new Vertex(this);
        graphicsScene->addItem(vertex);
        vertex->setPos(position);
        connect(vertex, &Vertex::vertexClicked, this, &GraphArea::onVertexClicked);
        connect(vertex, &Vertex::promptCreatePair, this, &GraphArea::onPromptCreatePair);
        connect(vertex, &Vertex::destroyVertex, this, &GraphArea::onDestroyVertex);
        adjacencyList.push_back(vertex);
    }

    if (adjacencyList.size() > std::numeric_limits<int>::max()) {
        qDebug() << "Too many edges, overflowing";
        return;
    }
    std::vector<std::pair<Vertex*, Vertex*>> vertexPairs;
    int N = static_cast<int>(adjacencyList.size());
    int maximum = N * (N - 1) / 2;
    int edgesToGenerate = static_cast<int>(density * maximum);

    while (edgesToGenerate != 0) {
        int weight = QRandomGenerator::global()->bounded(0, maxWeight + 1);
        int vertexAIndex = QRandomGenerator::global()->bounded(0, N);
        int vertexBIndex = QRandomGenerator::global()->bounded(0, N);
        Vertex* vertexA = adjacencyList[static_cast<unsigned>(vertexAIndex)];
        Vertex* vertexB = adjacencyList[static_cast<unsigned>(vertexBIndex)];
        std::pair<Vertex*, Vertex*> currentPair = std::make_pair(vertexA, vertexB);
        std::pair<Vertex*, Vertex*> currentPairRev = std::make_pair(vertexB, vertexA);
        bool notPaired = false;
        std::vector<std::pair<Vertex*, Vertex*>>::iterator iter = std::find(vertexPairs.begin(), vertexPairs.end(), currentPair);
        std::vector<std::pair<Vertex*, Vertex*>>::iterator iterRev = std::find(vertexPairs.begin(), vertexPairs.end(), currentPairRev);
        notPaired = (iter == vertexPairs.end() && iterRev == vertexPairs.end()) ? true : false;
        if (vertexAIndex != vertexBIndex && notPaired) {
            graphicsScene->addItem(new Edge(vertexA, vertexB, this, weight));
            vertexPairs.push_back(currentPair);
            vertexPairs.push_back(currentPairRev);
            --edgesToGenerate;
        }
    }
}

///////////////////// DEBUGGING HELPER START /////////////////////
///
void printEnum(int en) {
    switch (en) {
    case 0:
        qDebug() << "pointer" << QRandomGenerator::global()->generate();
        break;
    case 1:
        qDebug() << "vertex" << QRandomGenerator::global()->generate();
        break;
    case 2:
        qDebug() << "edge" << QRandomGenerator::global()->generate();
        break;
    case 3:
        qDebug() << "start" << QRandomGenerator::global()->generate();
        break;
    case 4:
        qDebug() << "started" << QRandomGenerator::global()->generate();
        break;
    case 5:
        qDebug() << "showpath" << QRandomGenerator::global()->generate();
        break;
    case 6:
        qDebug() << "calculated" << QRandomGenerator::global()->generate();
        break;
    }
}
///////////////////// DEBUGGING HELPER END /////////////////////

/**
 * @brief Detects mousePressEvent, specifically position.
 * @param event Mouse press event.
 */
void GraphArea::mousePressEvent(QMouseEvent* event)
{
    // Pass to all else as well.
    QGraphicsView::mousePressEvent(event);
    QGraphicsView::mousePressEvent(event);

    // DEBUGGING PURPOSES
    printEnum(static_cast<int>(cursor));
    // DEBUGGING PURPOSES

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
        startVertex = nullptr;
        break;
    case Cursor::CALCULATED:
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
        vertex->changeColor(Qt::blue);
        vertex->setDistance(0);
        emit turnOffStartButton();
    }
    else if (cursor == Cursor::STARTED) {
        emit turnOffStartButton();
        startAlgorithm();
    }
    else if (cursor == Cursor::CALCULATED) {
        emit turnOffStartButton();
    }
    else if (cursor == Cursor::SHOWPATH) {
        if (!isCalculated) {
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
                it->second->setLineColor(Qt::blue);
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
    dijkstraSourceVertex->setInPriorityQueue(true);

    while (!minHeap->isEmpty()) {
        Vertex* minDist = minHeap->findMin();
        if (minDist != nullptr) {
            if (unvisitedVertices->exists(minDist->getID())) {
                actionsList.push_back(Action(Command::CURRENTVERTEX, minDist, dijkstraCurrentVertex, nullptr, minDist->getDistance()));
                dijkstraCurrentVertex = minDist;
                unvisitedVertices->removeNode(minDist->getID());
            }
//            minDist->changeColor(Qt::gray);   // Original version no bonus
            minHeap->remove(minDist);
            minDist->setInPriorityQueue(false);
        }

        list<pair<Vertex*, Edge*>>& currentPairsList = dijkstraCurrentVertex->pairs();
        for (list<pair<Vertex*, Edge*>>::iterator it = currentPairsList.begin(); it != currentPairsList.end(); ++it) {
            // If visited, skip
            if (!unvisitedVertices->exists(it->first->getID())) {
                actionsList.push_back(Action(Command::VISITEDVERTEX, it->first, nullptr, nullptr, it->first->getDistance()));
                continue;
            }

            // Find neighbor vertex, with the edge connecting them
            int currentNeighborDistance = it->first->getDistance();
            int potentialNewDist = it->second->getWeight() + dijkstraCurrentVertex->getDistance();
            if (potentialNewDist < currentNeighborDistance) {
                if (it->first->getInPriorityQueue()) {
                    minHeap->remove(it->first);
                    it->first->setInPriorityQueue(false);
                    it->first->setDistance(potentialNewDist);
                    it->first->setPreviousVertex(dijkstraCurrentVertex);
                    actionsList.push_back(Action(Command::UPDATEDINHEAP, it->first, nullptr, it->second, it->first->getDistance()));
                }
                else {
                    it->first->setDistance(potentialNewDist);
                    it->first->setPreviousVertex(dijkstraCurrentVertex);
                    actionsList.push_back(Action(Command::VERTEXUPDATE, it->first, nullptr, it->second, it->first->getDistance()));
                }
                minHeap->insert(it->first);
                it->first->setInPriorityQueue(true);
            }
            else {
                actionsList.push_back(Action(Command::VERTEXNOUPDATE, it->first, nullptr, it->second, it->first->getDistance()));
            }
        }
    }
    cursor = Cursor::CALCULATED;
    isCalculated = true;
//    actionsList.pop_front();
    actionsList.push_back(Action(Command::CLEARALLVERTEX));
}

void GraphArea::clearColoredEdges()
{
    for (Edge* e : coloredEdges) {
        e->setLineColor(Qt::black);
        e->update();
    }
    coloredEdges.clear();
}

void GraphArea::clearAlgorithm()
{
    // Reset graph area
    clearColoredEdges();
    startVertex = nullptr;
    delete minHeap;
    minHeap = nullptr;
    delete unvisitedVertices;
    unvisitedVertices = nullptr;
    dijkstraSourceVertex = nullptr;
    dijkstraCurrentVertex = nullptr;
    dijkstraDestinationVertex = nullptr;
    isCalculated = false;
    isVisualized = false;
    actionsList.clear();

    // Reset all vertices
    for (Vertex* v : adjacencyList) {
        v->setID(-1);
        v->setDistance(std::numeric_limits<int>::max());
        v->setPreviousVertex(nullptr);
        v->setInPriorityQueue(false);
        v->changeColor(Qt::black);
        v->update();
    }
    update();
}

// Extraneous function, to implement new (not in the grading scheme).
void GraphArea::newSlate()
{
    clearAlgorithm();
    removeAndDeallocVertexEdge();
    adjacencyList.clear();
    update();
}

// Extraneous function, to implement new (not in the grading scheme).
void GraphArea::removeAndDeallocVertexEdge()
{
    std::vector<Edge*> edgesToRemove;
    for (Vertex* v : adjacencyList) {
        list<pair<Vertex*, Edge*>>& currentPairsList = v->pairs();
        for (list<pair<Vertex*, Edge*>>::iterator it = currentPairsList.begin(); it != currentPairsList.end(); ++it) {
            std::vector<Edge*>::iterator it2 = std::find(edgesToRemove.begin(), edgesToRemove.end(), it->second);
            if (it2 == edgesToRemove.end()) {
                edgesToRemove.push_back(it->second);
            }
        }
        delete v;
    }
    for (Edge* e : edgesToRemove) {
        delete e;
    }
}
