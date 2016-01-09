#include "arduinocontroller.h"

#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

ArduinoController::ArduinoController()
{
    is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;
    serialBuffer = "";

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_due_verndor_id){
                if(serialPortInfo.productIdentifier() == arduino_due_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    is_available = true;
                }
            }
        }
    }

    if (is_available) {
        // open and configure the serialport
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        QObject::connect(arduino, SIGNAL(readyRead()),this, SLOT(readSerial()));
    }
}

void ArduinoController::readSerial()
{
    QStringList bufferSplit = serialBuffer.split(",");
    if(bufferSplit.length() < 3){
        serialData = arduino->readAll();
        serialBuffer += QString::fromStdString(serialData.toStdString());
    }else{
        qDebug() << "FSR1: " << bufferSplit[0].toDouble() << ", FSR2: " << bufferSplit[1].toDouble();
        processRawInput(bufferSplit[0].toDouble() / 1024.0, bufferSplit[1].toDouble() / 1024.0);
        serialBuffer = "";
    }
}

ArduinoController::~ArduinoController()
{
    if(arduino->isOpen()){
        arduino->close();
    }
}

bool ArduinoController::isAvailable()
{
    return is_available;
}
