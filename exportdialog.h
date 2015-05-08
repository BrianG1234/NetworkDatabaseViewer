#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();
    quint64 getStartDateTime(){return fromDate;}
    quint64 getToDateTime(){return toDate;}
    QString getFileName(){return fileName;}

private:
    Ui::ExportDialog *ui;
    quint64 fromDate;
    quint64 toDate;
    QString fileName;

public slots:
    void enableDates();
    void updateFromDate();
    void updateToDate();
    void disableDates();
    void browseFile();

};

#endif // EXPORTDIALOG_H
