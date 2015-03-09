#include <QtGui/QApplication>
#include <QThread>
#include <QTextCodec>

#include "mainwindow.h"
#include "pageparser.h"
#include "worker.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    QThread* thread = new QThread;
    Worker* worker = new Worker();
    worker->moveToThread(thread);

    QObject::connect(&w, SIGNAL(start(QString,QString,int,int)), worker, SLOT(runParsing(QString,QString,int,int)));
    QObject::connect(&w, SIGNAL(stop()), worker, SLOT(stopParsing()));
    QObject::connect(&w, SIGNAL(pause()), worker, SLOT(onPause()));
    QObject::connect(&w, SIGNAL(resume()), worker, SLOT(onResume()));

    QObject::connect(worker, SIGNAL(workerStopped()), &w, SLOT(onWorkerStopped()));
    QObject::connect(worker, SIGNAL(workerPaused()), &w, SLOT(onWorkerPaused()));
    QObject::connect(worker, SIGNAL(listsChanged(QString, int)), &w, SLOT(updateLists(QString, int)));

    thread->start();

    return a.exec();
}
