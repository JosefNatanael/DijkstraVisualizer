#include "RandomGenDialog.h"
#include "ui_RandomGenDialog.h"

RandomGenDialog::RandomGenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RandomGenDialog)
{
    ui->setupUi(this);
}

RandomGenDialog::~RandomGenDialog()
{
    delete ui;
}

void RandomGenDialog::getValues(int &verticesRef, double &densityRef, int &maxWeightRef)
{
    verticesRef = ui->numVerticesSpinBox->value();
    densityRef = ui->densitySpinBox->value();
    maxWeightRef = ui->maxEdgeWeightSpinBox->value();
}
