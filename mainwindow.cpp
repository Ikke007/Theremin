#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    arduino_is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;
    serialBuffer = "";

    /*
    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Prodcut ID: " << serialPortInfo.productIdentifier();
        }
    }
    */

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_due_verndor_id){
                if(serialPortInfo.productIdentifier() == arduino_due_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }

    if(arduino_is_available){
        // open and configure the serialport
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        QObject::connect(arduino, SIGNAL(readyRead()),this, SLOT(readSerial()));

    }else{
        // give error message if not available
        QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");

        // .. and use XBox Controller instead
        controller = new CXBOXController(1);
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(controllerInput()));
        timer->start(100);
    }
}

MainWindow::~MainWindow()
{
    if(arduino->isOpen()){
        arduino->close();
    }
    delete ui;
    if(controller != NULL)
        delete(controller);
}


void MainWindow::controllerInput()
{
    if(controller->IsConnected())
    {
        processRawData(controller->GetState().Gamepad.sThumbRY * 100 / 32780,
                       controller->GetState().Gamepad.sThumbLY * 100 / 32780);
    }
    else
    {
        qDebug() << "\n\tERROR! PLAYER 1 - XBOX 360 Controller Not Found!\n";
        QCoreApplication::exit(1);
    }
}

void MainWindow::readSerial(){

    QStringList bufferSplit = serialBuffer.split(",");
    if(bufferSplit.length() < 2){
        serialData = arduino->readAll();
        serialBuffer += QString::fromStdString((serialData.toStdString()));
    }else{
        qDebug() << "FSR1: " << bufferSplit[0] << ", FSR2: " << bufferSplit[1];
        processRawData(bufferSplit[0].toInt(), bufferSplit[1].toInt());
        serialBuffer = "";
    }
}

// TODO auf irgendeinen Wertebereich festlegen? Zum testen hab ich 0 bis 100 genommen, das ist wohl ein bisschen klein ;)
void MainWindow::processRawData(int pitch, int volume)
{
    if (ui->checkBox_invert->isChecked())
    {
        int temp = pitch;
        pitch = volume;
        volume = temp;
    }
    ui->progressBar->setValue(pitch);
    ui->lcdNumber->display(pitch);
    ui->progressBar_2->setValue(volume);
    ui->lcdNumber_2->display(volume);

    // TODO generate MIDI data here
}

