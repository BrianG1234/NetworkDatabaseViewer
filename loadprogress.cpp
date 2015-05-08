#include "loadprogress.h"
#include "ui_loadprogress.h"

LoadProgress::LoadProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadProgress)
{
    ui->setupUi(this);

}

LoadProgress::~LoadProgress()
{
    delete ui;
}

void LoadProgress::setProgress(int inProgress)
{
    ui->progressBar->setValue(inProgress);   

}

void LoadProgress::setMessage(QString inString)
{
    ui->lblLoading->setText(inString);
}
