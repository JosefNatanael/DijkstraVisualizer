#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "GraphArea.h"
#include "Vertex.h"
#include "Edge.h"

#include <QMessageBox>
#include <QDebug>

/**
 * @brief MainWindow::MainWindow constructor.
 * @param parent Passed to QMainWindow() constructor.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set buttons to be checkable
    ui->drawVertexButton->setCheckable(true);
    ui->drawVertexButton->setStyleSheet("QPushButton:checked{background-color:lightgreen;}");
    ui->drawEdgeButton->setCheckable(true);
    ui->drawEdgeButton->setStyleSheet("QPushButton:checked{background-color:lightgreen;}");
    ui->startButton->setCheckable(true);
    ui->startButton->setStyleSheet("QPushButton:checked{background-color:lightgreen;}");
    ui->showPathButton->setCheckable(true);
    ui->showPathButton->setStyleSheet("QPushButton:checked{background-color:lightgreen;}");

    // Connections
    connect(ui->graphArea, &GraphArea::turnOffStartButton, this, &MainWindow::onTurnOffStartButton);
    connect(ui->graphArea, &GraphArea::turnOffShowPathButton, this, &MainWindow::onTurnOffShowPathButton);
}

/**
 * @brief MainWindow::~MainWindow destructor.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Exits the application.
 */
void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

/**
 * @brief Changes cursor mode to draw vertex.
 */
void MainWindow::on_drawVertexButton_clicked()
{
    GraphArea::Cursor cursor = ui->graphArea->getCursorMode();
    // Sets every other button to release and clear any colored edges
    ui->graphArea->clearColoredEdges();
    ui->startButton->setChecked(false);
    ui->drawEdgeButton->setChecked(false);
    ui->showPathButton->setChecked(false);

    // Turn off startVertex color
    if (ui->graphArea->startVertex != nullptr) {
        ui->graphArea->startVertex->changeColor(Qt::black);
        ui->graphArea->startVertex->update();
    }

    if (ui->graphArea->isCalculated) {
        if (cursor == GraphArea::Cursor::SHOWPATH) {
            ui->graphArea->setCursorMode(GraphArea::Cursor::CALCULATED);
        }
        return;
    }


    if (ui->drawVertexButton->isChecked() && cursor != GraphArea::Cursor::STARTED) {
        ui->graphArea->setCursorMode(GraphArea::Cursor::VERTEX);
    }
    else {
        ui->graphArea->setCursorMode(GraphArea::Cursor::POINTER);
    }
}

/**
 * @brief Changes cursor mode to draw edge.
 */
void MainWindow::on_drawEdgeButton_clicked()
{
    GraphArea::Cursor cursor = ui->graphArea->getCursorMode();
    // Sets every other button to release and clear any colored edges
    ui->graphArea->clearColoredEdges();
    ui->startButton->setChecked(false);
    ui->drawVertexButton->setChecked(false);
    ui->showPathButton->setChecked(false);

    // Turn off startVertex color
    if (ui->graphArea->startVertex != nullptr) {
        ui->graphArea->startVertex->changeColor(Qt::black);
        ui->graphArea->startVertex->update();
    }

    if (ui->graphArea->isCalculated) {
        if (cursor == GraphArea::Cursor::SHOWPATH) {
            ui->graphArea->setCursorMode(GraphArea::Cursor::CALCULATED);
        }
        return;
    }


    if (ui->drawEdgeButton->isChecked() && cursor != GraphArea::Cursor::STARTED) {
        ui->graphArea->setCursorMode(GraphArea::Cursor::EDGE);
    }
    else {
        ui->graphArea->setCursorMode(GraphArea::Cursor::POINTER);
    }
}

void MainWindow::on_startButton_clicked()
{
    GraphArea::Cursor cursor = ui->graphArea->getCursorMode();
    // Sets every other button to release and clear any colored edges
    ui->graphArea->clearColoredEdges();
    ui->drawEdgeButton->setChecked(false);
    ui->drawVertexButton->setChecked(false);
    ui->showPathButton->setChecked(false);

    // Turn off startVertex color
    if (ui->graphArea->startVertex != nullptr) {
        ui->graphArea->startVertex->changeColor(Qt::black);
        ui->graphArea->startVertex->update();
    }

    if (ui->graphArea->isCalculated) {
        if (cursor == GraphArea::Cursor::SHOWPATH) {
            ui->graphArea->setCursorMode(GraphArea::Cursor::CALCULATED);
        }
        return;
    }

    if (ui->startButton->isChecked() && ui->graphArea->getCursorMode() != GraphArea::Cursor::STARTED) {
        ui->graphArea->setCursorMode(GraphArea::Cursor::START);
    }
    else if (ui->graphArea->getCursorMode() != GraphArea::Cursor::STARTED) {
        ui->graphArea->setCursorMode(GraphArea::Cursor::POINTER);
    }
}

void MainWindow::on_showPathButton_clicked()
{
    GraphArea::Cursor cursor = ui->graphArea->getCursorMode();
    // Sets every other button to release
    ui->startButton->setChecked(false);
    ui->drawEdgeButton->setChecked(false);
    ui->drawVertexButton->setChecked(false);

    // Turn off startVertex color
    if (ui->graphArea->startVertex != nullptr) {
        ui->graphArea->startVertex->changeColor(Qt::black);
        ui->graphArea->startVertex->update();
    }

    // Calculated and show path, then turn off, return
    if (ui->graphArea->isCalculated && cursor == GraphArea::Cursor::SHOWPATH) {
        ui->graphArea->setCursorMode(GraphArea::Cursor::CALCULATED);
        ui->graphArea->clearColoredEdges();
        return;
    }

    ui->graphArea->setCursorMode(GraphArea::Cursor::SHOWPATH);

}

void MainWindow::onTurnOffStartButton()
{
    ui->startButton->setChecked(false);
}

void MainWindow::onTurnOffShowPathButton()
{
    ui->showPathButton->setChecked(false);
}

void MainWindow::on_clearButton_clicked()
{
    // Sets every other button to release
    ui->startButton->setChecked(false);
    ui->drawEdgeButton->setChecked(false);
    ui->drawVertexButton->setChecked(false);
    ui->showPathButton->setChecked(false);

    // Sets cursor to pointer
    ui->graphArea->setCursorMode(GraphArea::Cursor::POINTER);

    // Clear algorithm
    ui->graphArea->clearAlgorithm();
}

void MainWindow::on_actionNew_triggered()
{
    // Sets every other button to release
    ui->startButton->setChecked(false);
    ui->drawEdgeButton->setChecked(false);
    ui->drawVertexButton->setChecked(false);
    ui->showPathButton->setChecked(false);

    // Sets cursor to pointer
    ui->graphArea->setCursorMode(GraphArea::Cursor::POINTER);

    ui->graphArea->newSlate();
}

void MainWindow::on_randomGenerateButton_clicked()
{
    ui->graphArea->randomGenerator();
}

void MainWindow::on_stepButton_clicked()
{
    // Sets every other button to release
    ui->startButton->setChecked(false);
    ui->drawEdgeButton->setChecked(false);
    ui->drawVertexButton->setChecked(false);
    ui->showPathButton->setChecked(false);

    if (ui->graphArea->isCalculated) {
        // If there is still actions in the action list
        if (!ui->graphArea->actionsList.empty()) {
            ui->graphArea->clearColoredEdges();
            GraphArea::Action act = ui->graphArea->actionsList.front();
            ui->graphArea->actionsList.pop_front();
            switch (act.commandType) {
            case GraphArea::Command::NONE:
                qDebug() << "Error: Action is wrongly constructed";
                break;
            case GraphArea::Command::CURRENTVERTEX:
            {
                if (act.previousVertex != nullptr) {
                    act.previousVertex->changeColor(Qt::gray);
                    act.previousVertex->update();
                }
                if (ui->graphArea->actionsList.size() > 1) {
                    act.vertexConsidered->changeColor(Qt::blue);
                }
                else {
                    act.vertexConsidered->changeColor(Qt::gray);
                }
                break;
            }
            case GraphArea::Command::VERTEXUPDATE:
                act.vertexConsidered->changeColor(Qt::green);
                act.edgeConsidered->setLineColor(Qt::blue);
                ui->graphArea->coloredEdges.push_back(act.edgeConsidered);
                break;
            case GraphArea::Command::VERTEXNOUPDATE:
                act.vertexConsidered->changeColor(Qt::red);
                act.edgeConsidered->setLineColor(Qt::blue);
                ui->graphArea->coloredEdges.push_back(act.edgeConsidered);
                break;
            case GraphArea::Command::VISITEDVERTEX:
                act.vertexConsidered->changeColor(Qt::gray);
                break;
            case GraphArea::Command::UPDATEDINHEAP:
                act.vertexConsidered->changeColor(Qt::cyan);
                act.edgeConsidered->setLineColor(Qt::blue);
                ui->graphArea->coloredEdges.push_back(act.edgeConsidered);
                break;
            }
            if (act.vertexConsidered != nullptr)
                act.vertexConsidered->update();
            if (act.edgeConsidered != nullptr)
                act.edgeConsidered->update();

            // If there is no more actions in the action list
            if (ui->graphArea->actionsList.empty()) {
                ui->graphArea->isVisualized = true;
            }
            ui->graphArea->update();
        }
        else {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Finished");
            msgBox.setText("The algorithm is finished.");
            msgBox.exec();
        }
    }
}

///////////////////// DEBUGGING HELPER /////////////////////
///
void printEn(int en) {
    switch (en) {
    case 0:
        qDebug() << "none";
        break;
    case 1:
        qDebug() << "currentvertex";
        break;
    case 2:
        qDebug() << "vertexupdate";
        break;
    case 3:
        qDebug() << "vertexnoupdate";
        break;
    case 4:
        qDebug() << "visitedvertex";
        break;
    case 5:
        qDebug() << "updatedinheap";
        break;
    }
}
///////////////////// DEBUGGING HELPER /////////////////////

// For Debugging purposes only
void MainWindow::on_actionDebug_only_triggered()
{
    for (auto i : ui->graphArea->actionsList) {
        printEn(static_cast<int>(i.commandType));
    }
}


