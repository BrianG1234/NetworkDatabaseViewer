#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QErrorMessage>
#include <QErrorMessage>
#include <QSortFilterProxyModel>
#include <QList>
#include <QSqlError>
#include <QDateTime>
#include "loadprogress.h"
#include "missingrecordreport.h"
#include "exportdialog.h"

namespace Ui {
class MainView;
}

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();
    bool bFlightMode;
    QString databasePath;
    void cleanupMemory();

private:
    Ui::MainView *ui;
    QSqlDatabase networkDatabase;

    QSortFilterProxyModel *sortedAccelFilter;
    QSortFilterProxyModel *sortedEPSFilter;
    QSortFilterProxyModel *sortedGyroFilter;
    QSortFilterProxyModel *sortedMAGFilter;
    QSortFilterProxyModel *sortedCommandFilter;
    QSortFilterProxyModel *sortedSOHFilter;

    QSqlQueryModel *accelDataQueryModel;
    QSqlQueryModel *epsDataQueryModel;
    QSqlQueryModel *gyroDataQueryModel;
    QSqlQueryModel *magDataQueryModel;
    QSqlQueryModel *sohDataQueryModel;
    QSqlQueryModel *cmdSequenceQueryModel;




public slots:
    void exportData();
    void refreshDatabase();
    void deleteDuplicates();
    void findMissingData();
    void loadCurrentDatabase();
};

#endif // MAINVIEW_H
