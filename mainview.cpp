#include "mainview.h"
#include "ui_mainview.h"
#include <QDebug>
#include <QFileInfo>
#include <QStandardItemModel>
#include "missingrecordreport.h"

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);

    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnLoadDatabse, SIGNAL(clicked()), this, SLOT(loadCurrentDatabase()));
    connect(ui->btnFindMissingRecords, SIGNAL(clicked()), this, SLOT(findMissingData()));
    connect(ui->btnExport, SIGNAL(clicked()), this, SLOT(exportData()));

    ui->tblAccelView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblCommands->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblEPSView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblGyroView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblMagView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblSOHView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tblAccelView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tblCommands->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tblEPSView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tblGyroView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tblMagView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tblSOHView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    accelDataQueryModel = NULL;
    epsDataQueryModel = NULL;
    gyroDataQueryModel = NULL;
    magDataQueryModel = NULL;
    sohDataQueryModel = NULL;
    cmdSequenceQueryModel = NULL;
    sortedAccelFilter = NULL;
    sortedEPSFilter = NULL;
    sortedGyroFilter = NULL;
    sortedMAGFilter = NULL;
    sortedCommandFilter = NULL;
    sortedSOHFilter = NULL;

}


MainView::~MainView()
{    
    cleanupMemory();
    delete ui;


}

void MainView::loadCurrentDatabase()
{
    QMessageBox msgBox;
    msgBox.setText("Please Select the Database to Load");
    QPushButton *flightButton = msgBox.addButton(tr("Flight Mode"), QMessageBox::ActionRole);
    QPushButton *emButton = msgBox.addButton(tr("EM Mode"), QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == flightButton)
    {
        // Flight
        bFlightMode = true;
        databasePath = "Z:/CommandFiles/Database/Flight/commandDB.sqlite";
    }
    else if(msgBox.clickedButton() == emButton)
    {
        // EM
        bFlightMode = false;
        databasePath = "Z:/CommandFiles/Database/EM/commandDBEM.sqlite";
    }

    if(!QSqlDatabase::contains("MAIN"))
    {
        networkDatabase = QSqlDatabase::addDatabase("QSQLITE", "MAIN");
        networkDatabase.setDatabaseName(databasePath);
    }
    else
    {
        networkDatabase = QSqlDatabase::database("MAIN");
        networkDatabase.setDatabaseName(databasePath);
    }


    QFileInfo checkFile(databasePath);
    if (!checkFile.exists())
    {
        QMessageBox::warning(this, "Could not fine Database File", "Check the Z: Drive Connection");
        return;
    }

    //Clean up memory in case this is a reload.
    cleanupMemory();

    QString sohSqlString = "SELECT * FROM SOHData";
    QString epsSqlString = "SELECT * FROM EPSData";
    QString gyroSqlString = "SELECT * FROM GyroData";
    QString magSqlString = "SELECT * FROM MagData";
    QString accelSqlString = "SELECT * FROM AccelData";
    QString commandSqlString = "SELECT * FROM CommandSequence";



    //Set up the queries for the Data
    accelDataQueryModel = new QSqlQueryModel();
    epsDataQueryModel = new QSqlQueryModel();
    gyroDataQueryModel = new QSqlQueryModel();
    magDataQueryModel = new QSqlQueryModel();
    sohDataQueryModel = new QSqlQueryModel();
    cmdSequenceQueryModel = new QSqlQueryModel();

    LoadProgress progressTracker;
    progressTracker.show();

    if(networkDatabase.open())
    {
        //Start with the Accel Data
        QSqlQuery *accelSqlQuery = new QSqlQuery(networkDatabase);
        accelSqlQuery->prepare(accelSqlString);
        if(accelSqlQuery->exec())
        {
            accelDataQueryModel->setQuery(*accelSqlQuery);
            while ( accelDataQueryModel->canFetchMore() )
            {
                accelDataQueryModel->fetchMore();
            }

            //Sort the Data
            //QSortFilterProxyModel *sortedAccelFilter = new QSortFilterProxyModel(this);
            sortedAccelFilter = new QSortFilterProxyModel(this);
            sortedAccelFilter->setSourceModel(accelDataQueryModel);
            sortedAccelFilter->sort(1, Qt::AscendingOrder);
            ui->tblAccelView->setModel(sortedAccelFilter);

            ui->lblAccelRecords->setText(QString::number(accelDataQueryModel->rowCount()) + " Records");
            ui->tblAccelView->resizeColumnsToContents();
        }
        else
        {
            QMessageBox::warning(this, "Accel Query Error", "An Error occured Querying the Accel Data Table");
        }

        delete accelSqlQuery;
        progressTracker.setProgress(16);
        qApp->processEvents();


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Load the EPS Table
        QSqlQuery *epsSqlQuery = new QSqlQuery(networkDatabase);
        epsSqlQuery->prepare(epsSqlString);
        if(epsSqlQuery->exec())
        {
            epsDataQueryModel->setQuery(*epsSqlQuery);
            while ( epsDataQueryModel->canFetchMore() )
            {
                epsDataQueryModel->fetchMore();
            }

            //Sort the Data
            sortedEPSFilter = new QSortFilterProxyModel(this);
            sortedEPSFilter->setSourceModel(epsDataQueryModel);
            sortedEPSFilter->sort(1, Qt::AscendingOrder);
            ui->tblEPSView->setModel(sortedEPSFilter);

            ui->lblEPSRecords->setText(QString::number(epsDataQueryModel->rowCount()) + " Records");
            ui->tblEPSView->resizeColumnsToContents();
        }
        else
        {
            QMessageBox::warning(this, "EPS Query Error", "An Error occured Querying the EPS Data Table");
        }

        delete epsSqlQuery;
        progressTracker.setProgress(32);
        qApp->processEvents();


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Load the Gyro Table Data
        QSqlQuery *gyroSqlQuery = new QSqlQuery(networkDatabase);
        gyroSqlQuery->prepare(gyroSqlString);
        if(gyroSqlQuery->exec())
        {
            gyroDataQueryModel->setQuery(*gyroSqlQuery);
            while ( gyroDataQueryModel->canFetchMore() )
            {
                gyroDataQueryModel->fetchMore();
            }

            //Sort the Data
            sortedGyroFilter = new QSortFilterProxyModel(this);
            sortedGyroFilter->setSourceModel(gyroDataQueryModel);
            sortedGyroFilter->sort(1, Qt::AscendingOrder);
            ui->tblGyroView->setModel(sortedGyroFilter);

            ui->lblGyroRecords->setText(QString::number(gyroDataQueryModel->rowCount()) + " Records");
            ui->tblGyroView->resizeColumnsToContents();
        }
        else
        {
            QMessageBox::warning(this, "Gyro Query Error", "An Error occured Querying the Gyro Data Table");
        }

        delete gyroSqlQuery;
        progressTracker.setProgress(48);
        qApp->processEvents();


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Load the MAG Data
        QSqlQuery *magSqlQuery = new QSqlQuery(networkDatabase);
        magSqlQuery->prepare(magSqlString);
        if(magSqlQuery->exec())
        {
            magDataQueryModel->setQuery(*magSqlQuery);
            while ( magDataQueryModel->canFetchMore() )
            {
                magDataQueryModel->fetchMore();
            }

            //Sort the Data
            sortedMAGFilter = new QSortFilterProxyModel(this);
            sortedMAGFilter->setSourceModel(magDataQueryModel);
            sortedMAGFilter->sort(1, Qt::AscendingOrder);
            ui->tblMagView->setModel(sortedMAGFilter);

            ui->lblMAGRecords->setText(QString::number(magDataQueryModel->rowCount()) + " Records");
            ui->tblMagView->resizeColumnsToContents();
        }
        else
        {
            QMessageBox::warning(this, "MAG Query Error", "An Error occured Querying the MAG Data Table");
        }

        delete magSqlQuery;
        progressTracker.setProgress(64);
        qApp->processEvents();


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Load Command History
        QSqlQuery *commandSqlQuery = new QSqlQuery(networkDatabase);
        commandSqlQuery->prepare(commandSqlString);
        if(commandSqlQuery->exec())
        {
            cmdSequenceQueryModel->setQuery(*commandSqlQuery);
            while ( cmdSequenceQueryModel->canFetchMore() )
            {
                cmdSequenceQueryModel->fetchMore();
            }

            //Sort the Data
            sortedCommandFilter = new QSortFilterProxyModel(this);
            sortedCommandFilter->setSourceModel(cmdSequenceQueryModel);
            sortedCommandFilter->sort(1, Qt::AscendingOrder);
            ui->tblCommands->setModel(sortedCommandFilter);

            ui->lblCommandRecords->setText(QString::number(cmdSequenceQueryModel->rowCount()) + " Records");
            ui->tblCommands->resizeColumnsToContents();
        }
        else
        {
            QMessageBox::warning(this, "Command List Query Error", "An Error occured Querying the Command List Table");
        }

        delete commandSqlQuery;
        progressTracker.setProgress(80);
        qApp->processEvents();

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Load the SOH Data Table
        QSqlQuery *sohSqlQuery = new QSqlQuery(networkDatabase);
        sohSqlQuery->prepare(sohSqlString);
        if(sohSqlQuery->exec())
        {
            sohDataQueryModel->setQuery(*sohSqlQuery);
            while ( sohDataQueryModel->canFetchMore() )
            {
                sohDataQueryModel->fetchMore();
            }

            //Sort the Data
            sortedSOHFilter = new QSortFilterProxyModel(this);
            sortedSOHFilter->setSourceModel(sohDataQueryModel);
            sortedSOHFilter->sort(1, Qt::AscendingOrder);
            ui->tblSOHView->setModel(sortedSOHFilter);

            ui->lblSOHRecords->setText(QString::number(sohDataQueryModel->rowCount()) + " Records");
            ui->tblSOHView->resizeColumnsToContents();
        }
        else
        {
            QMessageBox::warning(this, "Command List Query Error", "An Error occured Querying the Command List Table");
        }

        delete sohSqlQuery;
        progressTracker.setProgress(100);
        qApp->processEvents();
    }
    else
    {
        QMessageBox::warning(this, "Could not open the network database", "Please check the Z: Drive Connection");
    }


    networkDatabase.close();
}

void MainView::exportData()
{
    ExportDialog exportDataGUI;
    int response = exportDataGUI.exec();
    if(response != QDialog::Rejected)
    {
        QString exportFileName = exportDataGUI.getFileName();
        quint64 startDate = exportDataGUI.getStartDateTime();
        quint64 endDate = exportDataGUI.getToDateTime();
        qDebug() << exportFileName;
        QFile csvFile(exportFileName);
        if(csvFile.open(QIODevice::WriteOnly))
        {
            QTextStream outStream(&csvFile);

            if(startDate == endDate)  //Means all dates were selected
            {
                QAbstractItemModel *model;
                //Export all data
                if( ui->tabWidgetMain->currentIndex() == 0 ) //SOH DATA
                {
                   model = ui->tblSOHView->model();

                }
                else if (ui->tabWidgetMain->currentIndex() == 1 ) //EPS
                {
                    model = ui->tblEPSView->model();

                }
                else if(ui->tabWidgetMain->currentIndex() == 2) //MAG
                {
                    model = ui->tblMagView->model();
                }
                else if (ui->tabWidgetMain->currentIndex() == 3) //Accel
                {
                    model = ui->tblAccelView->model();
                }
                else if (ui->tabWidgetMain->currentIndex() == 4) //Gyro
                {
                    model = ui->tblGyroView->model();
                }
                else
                {
                    model = ui->tblCommands->model();
                }

                QString tempString;

                int rows = model->rowCount();
                int cols = model->columnCount();
                qDebug() << "Rows " << rows << " Colums " << cols;

                for(int row_ctr = 0; row_ctr < rows; row_ctr++)
                {
                    tempString.clear();
                    for (int col_ctr = 0; col_ctr < cols; col_ctr++)
                    {
                        QModelIndex index = model->index(row_ctr, col_ctr);
                        tempString.append(model->data(index).toString() + ",");
                    }
                    tempString.remove(tempString.size()-1, 1);
                    tempString.append("\n");
                    outStream << tempString;
                }
                csvFile.close();
                QMessageBox::information(this, "Data Export Complete", exportFileName + " Complete");

            }
            else
            {
                //Could not create file
                QAbstractItemModel *model;
                //Export all data
                if( ui->tabWidgetMain->currentIndex() == 0 ) //SOH DATA
                {
                   model = ui->tblSOHView->model();

                }
                else if (ui->tabWidgetMain->currentIndex() == 1 ) //EPS
                {
                    model = ui->tblEPSView->model();

                }
                else if(ui->tabWidgetMain->currentIndex() == 2) //MAG
                {
                    model = ui->tblMagView->model();
                }
                else if (ui->tabWidgetMain->currentIndex() == 3) //Accel
                {
                    model = ui->tblAccelView->model();
                }
                else if (ui->tabWidgetMain->currentIndex() == 4) //Gyro
                {
                    model = ui->tblGyroView->model();
                }
                else
                {
                    model = ui->tblCommands->model();
                }

                QString tempString;

                int rows = model->rowCount();
                int cols = model->columnCount();

                for(int row_ctr = 0; row_ctr < rows; row_ctr++)
                {
                    tempString.clear();
                    for (int col_ctr = 0; col_ctr < 1; col_ctr++)
                    {
                        QModelIndex index = model->index(row_ctr, 1);
                        if(model->data(index).toInt() >= startDate && model->data(index).toInt() <= endDate)
                        {
                            for (int col_ctr2 = 0; col_ctr2 < cols; col_ctr2++)
                            {
                                QModelIndex index2 = model->index(row_ctr, col_ctr2);
                                tempString.append(model->data(index2).toString() + ",");
                            }
                            tempString.remove(tempString.size()-1, 1);
                            tempString.append("\n");
                            outStream << tempString;
                            tempString.clear();
                            qDebug() << "TimeStamp " << model->data(index).toInt();
                        }
                    }
                }
                csvFile.close();
                QMessageBox::information(this, "Data Export Complete", exportFileName + " Complete");
            }

        }
        else
        {
            //File failed to open
            QMessageBox::warning(this, "Failed to Create File", "Please check permissions of the write directory");
        }

    }
    else
    {
        //Cancel Selected
        return;
    }
}

void MainView::refreshDatabase()
{
    loadCurrentDatabase();
}


void MainView::deleteDuplicates()
{
    return;
}

void MainView::findMissingData()
{
    bool success = networkDatabase.open();
    if(success == false)
    {
        const QSqlError error = networkDatabase.lastError();
        qDebug() << "Error is " << error.text();
    }
    else
    {
        QString sqlString;
        QString typeOfReport;
        QList<qint64> timeRecords;
        QStringList missingTimeStamps;

        QSqlQuery query(networkDatabase);
        if( ui->tabWidgetMain->currentIndex() == 0 ) //SOH DATA
        {
            sqlString = "SELECT TimeStamp FROM SOHData";
            typeOfReport = "SOH";
        }
        else if (ui->tabWidgetMain->currentIndex() == 1 ) //EPS
        {
            sqlString = "SELECT TimeStamp FROM EPSData";
            typeOfReport = "EPS";
        }
        else if(ui->tabWidgetMain->currentIndex() == 2) //MAG
        {
            sqlString = "SELECT TimeStamp FROM MagData";
            typeOfReport = "MAG";

        }
        else if (ui->tabWidgetMain->currentIndex() == 3) //Accel
        {
            sqlString = "SELECT TimeStamp FROM AccelData";
            typeOfReport = "ACCEL";

        }
        else if (ui->tabWidgetMain->currentIndex() == 4) //Gyro
        {
            sqlString = "SELECT TimeStamp FROM GyroData";
            typeOfReport = "GYRO";

        }
        else
        {
            return;
        }

        query.prepare(sqlString);
        if(query.exec())
        {
            while(query.next())
            {

                timeRecords.append(query.value(0).toInt());
            }
        }

        qSort(timeRecords);
        quint64 tempValue = 0;
        quint64 tempDiff = 0;
        QString dateTimeStr;
        QString message;

        for(int ctr = 0; ctr < timeRecords.size()-1; ctr++)
        {

            if( qAbs((timeRecords.at(ctr) - timeRecords.at(ctr+1))) > 90)
            {
                dateTimeStr = QDateTime::fromTime_t(timeRecords.at(ctr+1)).toString();
                tempValue = timeRecords.at(ctr);
                tempDiff = qAbs((timeRecords.at(ctr) - timeRecords.at(ctr+1)) / 60);

                message = "GAP -- " + dateTimeStr + "  -- " + QString::number(tempValue) + "  Of " + QString::number(tempDiff) + " Records";

                missingTimeStamps.append(message);
                qDebug() << "GAP  - " << dateTimeStr << " -- " << timeRecords.at(ctr) << "Of " << (timeRecords.at(ctr) - timeRecords.at(ctr+1)) / 60 << "Records " << "Diff " << qAbs((timeRecords.at(ctr) - timeRecords.at(ctr+1)));
            }
        }

        if(missingTimeStamps.size() == 0)
        {
            missingTimeStamps.append("0 Missing Records Detected");
        }

        missingRecordReport tempGUI;
        tempGUI.setTitle(typeOfReport);
        tempGUI.setRecord(missingTimeStamps);
        tempGUI.exec();
    }
} //End Function


/////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainView::cleanupMemory
///
void MainView::cleanupMemory()
{
    //Check to see if this was reloaded - if so clean up first.
    if(accelDataQueryModel != NULL)
    {
        delete accelDataQueryModel;
        accelDataQueryModel = NULL;
    }

    if(epsDataQueryModel != NULL)
    {
        delete epsDataQueryModel;
        epsDataQueryModel = NULL;
    }

    if(gyroDataQueryModel != NULL)
    {
        delete gyroDataQueryModel;
        gyroDataQueryModel = NULL;
    }
    if(magDataQueryModel != NULL)
    {
        delete magDataQueryModel;
        magDataQueryModel = NULL;
    }
    if (sohDataQueryModel != NULL)
    {
        delete sohDataQueryModel;
        sohDataQueryModel = NULL;
    }
    if(cmdSequenceQueryModel != NULL)
    {
        delete cmdSequenceQueryModel;
        cmdSequenceQueryModel = NULL;
    }

    if(sortedAccelFilter != NULL)
    {
        delete sortedAccelFilter;
        sortedAccelFilter = NULL;
    }

    if(sortedCommandFilter != NULL)
    {
        delete sortedCommandFilter;
        sortedCommandFilter = NULL;
    }

    if(sortedEPSFilter != NULL)
    {
        delete sortedEPSFilter;
        sortedEPSFilter = NULL;
    }
    if(sortedGyroFilter != NULL)
    {
        delete sortedGyroFilter;
        sortedGyroFilter = NULL;
    }

    if(sortedMAGFilter != NULL)
    {
        delete sortedMAGFilter;
        sortedMAGFilter = NULL;
    }
    if(sortedSOHFilter != NULL)
    {
        delete sortedSOHFilter;
        sortedSOHFilter = NULL;
    }
}


