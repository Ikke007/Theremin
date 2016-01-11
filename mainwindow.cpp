#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup GraphViewWidget
    ui->graphWidget->addGraph(QColor(255,0,0));
    ui->graphWidget->addGraph(QColor(0,255,0));

    // hide DockWidgets
    ui->vibratoSettingsDock->setVisible(false);
    ui->midiSettingsDock->setVisible(false);
    ui->alternativeInputDock->setVisible(false);
    ui->inputSettingsDock->setVisible(false);

    QStringList connections = midiGenerator.connections();
    ui->midiDeviceComboBox->addItems(connections);

    for(int i = 0; i < connections.size(); i++)
    {
        if (midiGenerator.open(connections.at(i))) {
            ui->midiDeviceComboBox->setCurrentIndex(i);
            break;
        }
    }

    // apply default values
    midiGenerator.setMaxNote(ui->maxNote->value());
    midiGenerator.setMinNote(ui->minNote->value());
    midiGenerator.setProgram(ui->midiProgram->value());
    midiGenerator.setChannel(ui->midiChannel->value());
    midiGenerator.setVibratoRange(ui->sliderVibratoRange->value() / 100.0);
    midiGenerator.setVibratoSpeed(ui->sliderVibratoSpeed->value() / 100.0);

    // connect signals
    // arduino input
    QObject::connect(&arduino, SIGNAL(processRawInput(double, double)),
                     &midiGenerator, SLOT(processRawInput(double, double)));
    // alternative (slider) input
    QObject::connect(this, SIGNAL(sendRawInput(double,double)),
                     &midiGenerator, SLOT(processRawInput(double,double)));
    // callback after midi events have been created and send
    QObject::connect(&midiGenerator, SIGNAL(inputGenerated(double, double)),
                     this, SLOT(onInputGenerated(double, double)));


    if (!arduino.isAvailable()) {
        qDebug() << "Arduino not found. Enabling alternative input.";
        ui->alternativeInputDock->setVisible(true);
        // start generating sound
        sendSliderInput();
    }

    midiGenerator.start();
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
    Q_UNUSED(value)
    sendSliderInput();
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    Q_UNUSED(value)
    sendSliderInput();
}

void MainWindow::onInputGenerated(double frequency, double volume)
{
    // adjust display
    ui->graphWidget->setValue(0, frequency);
    ui->graphWidget->setValue(1, volume);
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
    ui->midiSettingsDock->raise();
}

void MainWindow::on_actionVibratoSettings_triggered(bool checked)
{
    ui->vibratoSettingsDock->setVisible(checked);
    ui->vibratoSettingsDock->raise();
}

void MainWindow::on_vibratoSettingsDock_visibilityChanged(bool visible)
{
    ui->actionVibratoSettings->setChecked(visible);
}

void MainWindow::on_midiSettingsDock_visibilityChanged(bool visible)
{
    ui->actionMidiSettings->setChecked(visible);
}

void MainWindow::on_actionAlternativeInput_triggered(bool checked)
{
    ui->alternativeInputDock->setVisible(checked);
    ui->alternativeInputDock->raise();
}

void MainWindow::on_alternativeInputDock_visibilityChanged(bool visible)
{
    ui->actionAlternativeInput->setChecked(visible);
}

void MainWindow::on_actionInputSettings_triggered(bool checked)
{
    ui->inputSettingsDock->setVisible(checked);
    ui->inputSettingsDock->raise();
}

void MainWindow::on_inputSettingsDock_visibilityChanged(bool visible)
{
    ui->actionInputSettings->setChecked(visible);
}

void MainWindow::on_midiChannel_valueChanged(int channel)
{
    midiGenerator.setChannel(channel);
    if (!arduino.isAvailable())
        sendSliderInput();
}

void MainWindow::on_midiDeviceComboBox_currentIndexChanged(const QString &text)
{
    if (ui->midiSettingsDock->isVisible() && !midiGenerator.open(text)) {
          QMessageBox::warning(this, "MIDI connection failure",
             "Connection \"" + text + "\" not available.");
      }
}
