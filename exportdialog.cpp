#include "exportdialog.h"
#include "ui_exportdialog.h"
#include <QDebug>
#include <QFileDialog>

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    connect(ui->rbtnSelectRanges, SIGNAL(clicked()), this, SLOT(enableDates()));
    connect(ui->rbtnData, SIGNAL(clicked()), this, SLOT(disableDates()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnExport, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(browseFile()));
    connect(ui->dateStart, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(updateFromDate()));
    connect(ui->dateEnd, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(updateToDate()));

    ui->groupBoxDateTime->setEnabled(false);
    ui->dateStart->setDateTime(QDateTime::currentDateTime());
    ui->dateEnd->setDateTime(QDateTime::currentDateTime());
    ui->rbtnData->setChecked(true);
    ui->btnExport->setEnabled(false);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::browseFile()
{
    fileName = QFileDialog::getSaveFileName(this, tr("Open File"), "", "CSV (*.csv)");
    ui->lnEditFileName->setText(fileName);
    ui->btnExport->setEnabled(true);

}

void ExportDialog::enableDates()
{
     ui->groupBoxDateTime->setEnabled(true);
}

void ExportDialog::disableDates()
{
    ui->groupBoxDateTime->setEnabled(false);
}

void ExportDialog::updateFromDate()
{
    fromDate = ui->dateStart->dateTime().toTime_t();
    qDebug() << "From Date Changed " << fromDate;
    return;
}

void ExportDialog::updateToDate()
{
    toDate = ui->dateEnd->dateTime().toTime_t();
    qDebug() << "TO Date Changed " << toDate;
}
