#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(&arduino, SIGNAL(processRawInput(double, double)),
                     this, SLOT(processRawInput(double, double)));

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
}

MainWindow::~MainWindow()
{
    delete ui;
}


// TODO auf irgendeinen Wertebereich festlegen? Zum testen hab ich 0 bis 100 genommen, das ist wohl ein bisschen klein ;)
// von 0 bis 1 ?
void MainWindow::processRawInput(double frequency, double volume)
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
    // display volume
    ui->progressBar_2->setValue(volume * 100);
    ui->lcdNumber_2->display(volume * 100);

    // generate MIDI data
    int newNote = frequency * 128;
    double pitch = frequency * 128 - newNote;

    midiOutput.sendPitchBend(midichannel, pitch * 4096);

    // fix for MS GS Wavetable Synth
    if (newNote < 0)
        newNote = -1;

    //midiOutput.sendPitchBend(midichannel, volume * 8192);
    if (activeNote != newNote) {
        midiOutput.sendNoteOn(midichannel, newNote, 127);
        midiOutput.sendNoteOff(midichannel, activeNote, 0);
        activeNote = newNote;
    }

    // send volume
    midiOutput.sendController(midichannel, 7, volume * 128);
}

void MainWindow::sendSliderInput() {
    processRawInput(ui->frequencySlider->value() / 10000.0,
                   ui->volumeSlider->value() / 1000.0);
}


void MainWindow::on_frequencySlider_valueChanged(int value)
{
    sendSliderInput();
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    sendSliderInput();
}
