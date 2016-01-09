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
    void onInputGenerated(double frequency, double volume);

    void on_frequencySlider_valueChanged(int value);
    void on_volumeSlider_valueChanged(int value);
    void on_checkBox_invert_toggled(bool checked);
    void on_midiProgram_valueChanged(int arg1);

    void on_minNote_valueChanged(int arg1);

    void on_maxNote_valueChanged(int arg1);

    void on_sliderVibratoRange_valueChanged(int value);

    void on_sliderVibratoSpeed_valueChanged(int value);

    void on_actionMidiSettings_triggered(bool checked);

    void on_actionVibratoSettings_triggered(bool checked);

    void on_vibratoSettingsDock_visibilityChanged(bool visible);

    void on_midiSettingsDock_visibilityChanged(bool visible);

    void on_actionAlternativeInput_triggered(bool checked);

    void on_alternativeInputDock_visibilityChanged(bool visible);

    void on_actionInputSettings_triggered(bool checked);

    void on_inputSettingsDock_visibilityChanged(bool visible);

private:
    void sendSliderInput();

    Ui::MainWindow *ui;
    ArduinoController arduino;
    MidiGenerator midiGenerator;
};

#endif // MAINWINDOW_H
