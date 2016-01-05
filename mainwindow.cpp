#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initialize midi output

    // TODO add midi output selection
        //QStringList connections = midiOutput.connections(true);
        //ui->comboBox->addItems(connections);

    if (!midiOutput.open("CoolSoft VirtualMIDISynth"))
        midiOutput.open("Microsoft GS Wavetable Synth");

    // TODO let the user choose the midi channel
    midichannel = 0;
    midiOutput.sendProgram(midichannel, 81); // irgendein Synth Programm

    // eigentlich sollten das hier attack und release time sein,
    // aber da bin ich mir nicht so sicher...
    midiOutput.sendController(midichannel,72,40);
    midiOutput.sendController(midichannel,73,0);
    //midiOutput.sendController(midichannel,68,0);

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


#ifndef XCONTROLLER
        // give error message if not available
        QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
#else
        // .. and use XBox Controller instead
        controller = new CXBOXController(1);
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(controllerInput()));
        timer->start(10);
#endif

        /*for (int i = 5000000; i < 10000000; i++) {
            processRawData(i / 10000000.0, 20);
        }*/
    }
}

MainWindow::~MainWindow()
{
    if(arduino->isOpen()){
        arduino->close();
    }
    delete ui;
#ifdef XCONTROLLER
    if(controller != NULL)
        delete(controller);
#endif
}

#ifdef XCONTROLLER
void MainWindow::controllerInput()
{
    if(controller->IsConnected())
    {
        processRawData(controller->GetState().Gamepad.sThumbRY / 32780.0,
                       controller->GetState().Gamepad.sThumbLY / 32780.0);
    }
    else
    {
        qDebug() << "\n\tERROR! PLAYER 1 - XBOX 360 Controller Not Found!\n";
        QCoreApplication::exit(1);
    }
}
#endif

void MainWindow::readSerial(){

    QStringList bufferSplit = serialBuffer.split(",");
    if(bufferSplit.length() < 2){
        serialData = arduino->readAll();
        serialBuffer += QString::fromStdString((serialData.toStdString()));
    }else{
        qDebug() << "FSR1: " << bufferSplit[0] << ", FSR2: " << bufferSplit[1];
        processRawData(bufferSplit[0].toInt() / 1024.0, bufferSplit[1].toInt() / 1024.0);
        serialBuffer = "";
    }
}

// TODO auf irgendeinen Wertebereich festlegen? Zum testen hab ich 0 bis 100 genommen, das ist wohl ein bisschen klein ;)
// von 0 bis 1 ?
void MainWindow::processRawData(double frequency, double volume)
{
    if (ui->checkBox_invert->isChecked())
    {
        int temp = frequency;
        frequency = volume;
        volume = temp;
    }

    // display frequency
    ui->progressBar->setValue(frequency * 100);
    ui->lcdNumber->display(frequency * 100);


    // TODO generate MIDI data here
    int newNote = frequency * 128;
    double pitch = frequency * 128 - newNote;

    // fix for MS GS Wavetable Synth
    if (newNote < 0)
        newNote = -1;

    // display pitch bend value (not volume at the moment...)
    ui->progressBar_2->setValue(pitch * 100);
    ui->lcdNumber_2->display(pitch * 100);
    midiOutput.sendPitchBend(midichannel, pitch * 4096);
    //midiOutput.sendPitchBend(midichannel, volume * 8192);
    if (activeNote != newNote) {
        midiOutput.sendNoteOn(midichannel, newNote, 127);
        midiOutput.sendNoteOff(midichannel, activeNote, 0);
        activeNote = newNote;
    }
}

