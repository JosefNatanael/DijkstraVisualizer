#include "GraphArea.h"

/**
 * @brief GraphArea constructor.
 * @param parent Passed to QGraphicsView() constructor.
 */
GraphArea::GraphArea(QWidget *parent)
    : QGraphicsView(parent)
{

}

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
    // Right click.
    if (event->buttons() == Qt::RightButton) {
        return;
    }
    // Not left click, return.
    if (event->buttons() != Qt::LeftButton) {
        return;
    }
    switch (cursor) {
    case Cursor::POINTER:
        break;
    case Cursor::VERTEX:
        emit drawVertexClicked(event->pos());
        break;
    case Cursor::EDGE:
        emit drawEdgeClicked(event->pos());
        break;
    case Cursor::START:
        break;
    case Cursor::SHOWPATH:
        break;
    }
}
