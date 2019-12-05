#include "Visualizer.h"

/**
 * @brief Visualizer::Visualizer constructor.
 * @param visualizerArea Algorithm Visualizer area
 * @param statusBar Status bar.
 * @param parent Passed to QObject() constructor.
 */
Visualizer::Visualizer(GraphArea* graphArea, QStatusBar* statusBar, QObject *parent)
    : QObject(parent)
    , graphArea(graphArea)
    , statusBar(statusBar)
{
    // Connects signals from the visualizer area
    connect(graphArea, &GraphArea::drawVertexClicked, this, &Visualizer::addVertex);
}

void Visualizer::addVertex(QPoint pos)
{
    Vertex* label = new Vertex(pos.x() - 12, pos.y() - 12, graphArea);
    label->setAlignment(Qt::AlignCenter);
    label->setVisible(true);
    label->setGeometry(pos.x() - 12, pos.y() - 12, 25, 25);
    installEventFilter(label);
}

void Visualizer::addEdge(QPoint pos)
{

}
