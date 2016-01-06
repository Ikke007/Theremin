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
                     &midiGenerator, SLOT(processRawInput(double, double)));

    QObject::connect(this, SIGNAL(sendRawInput(double,double)),
                     &midiGenerator, SLOT(processRawInput(double,double)));

    QObject::connect(&midiGenerator, SIGNAL(inputGenerated(double, double)),
                     this, SLOT(onInputGenerated(double, double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendSliderInput() {
    sendRawInput(ui->frequencySlider->value() / 10000.0,
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

void MainWindow::onInputGenerated(double frequency, double volume)
{
    // display frequency
    ui->progressBar->setValue(frequency * 100);
    ui->lcdNumber->display(frequency * 100);
    // display volume
    ui->progressBar_2->setValue(volume * 100);
    ui->lcdNumber_2->display(volume * 100);
}

void MainWindow::on_checkBox_invert_toggled(bool checked)
{
    midiGenerator.setInvertInput(checked);
}

void MainWindow::on_midiProgram_valueChanged(int program)
{
    midiGenerator.setProgram(program);
}
