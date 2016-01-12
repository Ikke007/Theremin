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
    serialData = arduino->readAll();
    serialBuffer += QString::fromStdString(serialData.toStdString());
    //qDebug() << serialBuffer;

    QStringList bufferSplit = serialBuffer.split(";");
    if (bufferSplit.length() < 2) {
        //qDebug() << "no data";
    } else {
        for (int i = bufferSplit.length() - 2; i >= 0; i--) {
            QStringList dataSplit = bufferSplit[i].split(",");
            if (dataSplit.length() != 2 || dataSplit[0] == "") {
                //qDebug() << "invalid data: \"" << dataSplit << "\"";
            } else {
                qDebug() << "FSR1: " << dataSplit[0] << ", FSR2: " << dataSplit[1];
                processRawInput(dataSplit[0].toDouble() / 4096.0, dataSplit[1].toDouble() / 4096.0);
                serialBuffer = bufferSplit.last();
                return;
            }
        }
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
