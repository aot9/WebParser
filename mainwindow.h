#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

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
    void start(QString aUrl, QString aText, int aTreads, int aLinks);
    void stop();
    void pause();
    void resume();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();

public slots:
    void updateLists(QString aProcessedLink, int aMatchNum);
    void onWorkerStopped();
    void onWorkerPaused();

private:
    Ui::MainWindow *ui;
    int m_maxMatch;
};

#endif // MAINWINDOW_H
