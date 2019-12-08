#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_startButton_clicked();
    void on_showPathButton_clicked();
    void on_clearButton_clicked();

    void onTurnOffStartButton();
    void onTurnOffShowPathButton();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
