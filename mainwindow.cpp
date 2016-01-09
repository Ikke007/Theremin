#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->vibratoSettingsDock->setVisible(false);
    ui->midiSettingsDock->setVisible(false);

    QObject::connect(&arduino, SIGNAL(processRawInput(double, double)),
                     &midiGenerator, SLOT(processRawInput(double, double)));

    QObject::connect(this, SIGNAL(sendRawInput(double,double)),
                     &midiGenerator, SLOT(processRawInput(double,double)));

    QObject::connect(&midiGenerator, SIGNAL(inputGenerated(double, double)),
                     this, SLOT(onInputGenerated(double, double)));

    sendSliderInput();
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
    midiGenerator.generate();
}

void MainWindow::on_midiProgram_valueChanged(int program)
{
    midiGenerator.setProgram(program);
    midiGenerator.generate();
}

void MainWindow::on_minNote_valueChanged(int min)
{
    midiGenerator.setMinNote(min);
    if (min > ui->maxNote->value()) {
        ui->maxNote->setValue(min);
        midiGenerator.setMaxNote(min);
    }
    midiGenerator.generate();
}

void MainWindow::on_maxNote_valueChanged(int max)
{
    midiGenerator.setMaxNote(max);
    if (max < ui->minNote->value()) {
        ui->minNote->setValue(max);
        midiGenerator.setMinNote(max);
    }
    midiGenerator.generate();
}

void MainWindow::on_sliderVibratoRange_valueChanged(int value)
{
    midiGenerator.setVibratoRange(value / 100.0);
}

void MainWindow::on_sliderVibratoSpeed_valueChanged(int value)
{
    midiGenerator.setVibratoSpeed(value / 100.0);
}

void MainWindow::on_actionMidiSettings_triggered(bool checked)
{
    ui->midiSettingsDock->setVisible(checked);
}

void MainWindow::on_actionVibratoSettings_triggered(bool checked)
{
    ui->vibratoSettingsDock->setVisible(checked);
}

void MainWindow::on_vibratoSettingsDock_visibilityChanged(bool visible)
{
    ui->actionVibratoSettings->setChecked(visible);
}

void MainWindow::on_midiSettingsDock_visibilityChanged(bool visible)
{
    ui->actionMidiSettings->setChecked(visible);
}
