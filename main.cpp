#include <QtGui/QApplication>
#include "mainwindow.h"
#include <pageparser.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    PageParser p;
    p.run();
    return a.exec();
}
