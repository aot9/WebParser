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
    sharedinfo.cpp

HEADERS  += mainwindow.h \
    pageparser.h \
    worker.h \
    sharedinfo.h

FORMS    += mainwindow.ui
