#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "arduinocontroller.h"
#include "midigenerator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void sendRawInput(double frequency, double volume);

private slots:
    void on_frequencySlider_valueChanged(int value);
    void on_volumeSlider_valueChanged(int value);
    void onInputGenerated(double frequency, double volume);

    void on_checkBox_invert_toggled(bool checked);

private:
    void sendSliderInput();

    Ui::MainWindow *ui;
    ArduinoController arduino;
    MidiGenerator midiGenerator;
};

#endif // MAINWINDOW_H
