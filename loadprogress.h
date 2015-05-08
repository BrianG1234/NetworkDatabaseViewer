#ifndef LOADPROGRESS_H
#define LOADPROGRESS_H

#include <QDialog>

namespace Ui {
class LoadProgress;
}

class LoadProgress : public QDialog
{
    Q_OBJECT

public:
    explicit LoadProgress(QWidget *parent = 0);
    ~LoadProgress();
    void setProgress(int iProgress);
    void setMessage(QString inString);

private:
    Ui::LoadProgress *ui;


};

#endif // LOADPROGRESS_H
