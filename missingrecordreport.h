#ifndef MISSINGRECORDREPORT_H
#define MISSINGRECORDREPORT_H

#include <QDialog>

namespace Ui {
class missingRecordReport;
}

class missingRecordReport : public QDialog
{
    Q_OBJECT

public:
    explicit missingRecordReport(QWidget *parent = 0);
    ~missingRecordReport();
    void setRecord(QStringList inRecordData);
    void setTitle(QString inString);

private:
    Ui::missingRecordReport *ui;

};

#endif // MISSINGRECORDREPORT_H
