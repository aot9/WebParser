#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

class QStringListModel;
class Worker;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setWorkerPtr(Worker*);

signals:
    void startBtnClicked(QString url, QString text, int threads, int links);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

public slots:
    void updateLists(QString, int);
    void onWorkerFinished();

private:
    Ui::MainWindow *ui;
    QStringListModel *inProgressListModel, *completedListModel;
    bool isRunning;
    Worker* workerPtr;
};

#endif // MAINWINDOW_H
