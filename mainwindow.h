#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

#ifdef _MSC_VER
#define XCONTROLLER
#endif

#ifdef XCONTROLLER
#include "CXBOXController.h"
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void readSerial();
#ifdef XCONTROLLER
    void controllerInput();
#endif
    void processRawData(int pitch, int volume);


private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    static const quint16 arduino_due_verndor_id = 9025;
    static const quint16 arduino_due_product_id = 61;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray serialData;
    QString serialBuffer;
#ifdef XCONTROLLER
    CXBOXController* controller;
#endif
};

#endif // MAINWINDOW_H
