#ifndef RANDOMGENDIALOG_H
#define RANDOMGENDIALOG_H

#include <QDialog>

namespace Ui {
class RandomGenDialog;
}

class RandomGenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RandomGenDialog(QWidget *parent = nullptr);
    ~RandomGenDialog();
    void getValues(int& verticesRef, double& densityRef, int& maxWeightRef);

private slots:

private:
    Ui::RandomGenDialog *ui;
};

#endif // RANDOMGENDIALOG_H
