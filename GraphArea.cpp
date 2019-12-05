#include "GraphArea.h"
#include "Vertex.h"
#include "Edge.h"

#include <QDebug>
#include "Utilities/WindowHelper.h"
#include <QDesktopWidget>

/**
 * @brief GraphArea constructor.
 * @param parent Passed to QGraphicsView() constructor.
 */
GraphArea::GraphArea(QWidget *parent)
    : QGraphicsView(parent)
    , graphicsScene(new QGraphicsScene(this))
{
    setScene(graphicsScene);
    scene()->installEventFilter(this);
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
    for (Vertex* v : vertices) {
        delete v;
    }
}

/**
 * @brief Sets cursor.
 * @param cursor
 */
void GraphArea::setCursorMode(Cursor cursor)
{
    this->cursor = cursor;
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
        Vertex* vertex = new Vertex(this);
        graphicsScene->addItem(vertex);
        vertex->setPos(event->pos());
        connect(vertex, &Vertex::vertexClicked, this, &GraphArea::onVertexClicked);
        startVertex = nullptr;
        break;
    }
    case Cursor::START:
        startVertex = nullptr;
        break;
    case Cursor::SHOWPATH:
        startVertex = nullptr;
        break;
    }
}

void GraphArea::onVertexClicked(Vertex* vertex)
{
    if (cursor == Cursor::EDGE && startVertex == nullptr) {
        startVertex = vertex;
    }
    else if (cursor == Cursor::EDGE && startVertex != nullptr && startVertex != vertex) {
        graphicsScene->addItem(new Edge(startVertex, vertex));
        startVertex = nullptr;
    }
}
