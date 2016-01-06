#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "arduinocontroller.h"
#include "midioutput.h"


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
    void on_frequencySlider_valueChanged(int value);
    void on_volumeSlider_valueChanged(int value);
    void processRawInput(double pitch, double volume);

private:
    void sendSliderInput();

    Ui::MainWindow *ui;
    ArduinoController arduino;

    drumstick::rt::MIDIOutput midiOutput;
    int midichannel;
    int activeNote = -1;
};

#endif // MAINWINDOW_H
