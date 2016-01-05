#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T15:10:13
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Theremin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CXBOXController.cpp

HEADERS  += mainwindow.h \
    CXBOXController.h

FORMS    += mainwindow.ui

LIBS += -lXInput

include(drumstick/drumstick.pro)
