#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QObject>
#include <QStatusBar>
#include "GraphArea.h"
#include "Vertex.h"

class Visualizer : public QObject
{
    Q_OBJECT

public:
    explicit Visualizer(GraphArea* graphArea, QStatusBar *statusBar, QObject *parent = nullptr);

private:


private:
    GraphArea* graphArea;
    QStatusBar* statusBar;
    Vertex* startVertex;
};

#endif // VISUALIZER_H
