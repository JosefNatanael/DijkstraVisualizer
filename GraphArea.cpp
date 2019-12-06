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
//    for (Vertex* v : vertices) {
//        delete v;
//    }
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
//        vertices.push_back(vertex);

        // Sets startVertex to nullptr in case user cancels edge operation.
        startVertex = nullptr;
        break;
    }
    case Cursor::EDGE:
        break;
    case Cursor::START:
        startVertex = nullptr;
        break;
    case Cursor::SHOWPATH:
        startVertex = nullptr;
        break;
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
        startVertex->changeColor(Qt::yellow);
        startVertex->update();
    }
    else if (cursor == Cursor::EDGE && startVertex != nullptr) {
        // Basically tells the vertex to emit a signal that calls onPromptCreatePair, via mouse release.
        vertex->setCandidatePairFound(true);
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
