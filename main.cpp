#include <QtGui/QApplication>
#include "mainwindow.h"
#include <pageparser.h>
#include <QThread>
#include "worker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QThread* thread = new QThread;
    Worker* worker = new Worker();
    worker->moveToThread(thread);

    w.setWorkerPtr(worker);

    QObject::connect(&w, SIGNAL(startBtnClicked(QString,QString,int,int)), worker, SLOT(runParsing(QString,QString,int,int)));

    QObject::connect(worker, SIGNAL(listsChanged(QString, int)), &w, SLOT(updateLists(QString, int)));
    QObject::connect(worker, SIGNAL(workerFinished()), &w, SLOT(onWorkerFinished()));

    thread->start();

    return a.exec();
}
