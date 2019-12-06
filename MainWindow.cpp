#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "GraphArea.h"

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
    // Sets every other button to release
    ui->drawEdgeButton->setChecked(false);

    if (ui->drawVertexButton->isChecked()) {
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
    // Sets every other button to release
    ui->drawVertexButton->setChecked(false);

    if (ui->drawEdgeButton->isChecked()) {
        ui->graphArea->setCursorMode(GraphArea::Cursor::EDGE);
    }
    else {
        ui->graphArea->setCursorMode(GraphArea::Cursor::POINTER);
    }
}
