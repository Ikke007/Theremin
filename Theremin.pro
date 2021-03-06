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
    arduinocontroller.cpp \
    midigenerator.cpp \
    graphviewwidget.cpp

HEADERS  += mainwindow.h \
    arduinocontroller.h \
    midigenerator.h \
    graphviewwidget.h

FORMS    += mainwindow.ui

win32: RC_ICONS += theremin.ico

include(drumstick/drumstick.pro)
