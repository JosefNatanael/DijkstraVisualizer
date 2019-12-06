#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Visualizer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_drawVertexButton_clicked();
    void on_drawEdgeButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
