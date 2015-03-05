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

signals:
    void startBtnClicked(QString url, QString text, int threads, int links);
    void stopBtnPressed();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

public slots:
    void updateLists(QString, int);
    void onWorkerStopped();

private:
    Ui::MainWindow *ui;
    QStringListModel *inProgressListModel, *completedListModel;
    uint maxMatch;
};

#endif // MAINWINDOW_H
