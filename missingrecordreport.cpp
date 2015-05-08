#include "missingrecordreport.h"
#include "ui_missingrecordreport.h"

missingRecordReport::missingRecordReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::missingRecordReport)
{
    ui->setupUi(this);
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(close()));
}

missingRecordReport::~missingRecordReport()
{
    delete ui;
}

void missingRecordReport::setRecord(QStringList inRecordData)
{
    for(int ctr = 0; ctr < inRecordData.size(); ctr++)
    {
        ui->textRecordsReport->append(inRecordData.at(ctr));
    }
}

void missingRecordReport::setTitle(QString inString)
{
    ui->lblTitle->setText(inString);
}
