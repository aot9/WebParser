#-------------------------------------------------
#
# Project created by QtCreator 2015-03-03T11:27:19
#
#-------------------------------------------------

QT       += core gui network

TARGET = WebParser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pageparser.cpp \
    worker.cpp \
    linkqueue.cpp

HEADERS  += mainwindow.h \
    pageparser.h \
    worker.h \
    linkqueue.h

FORMS    += mainwindow.ui
