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
}
