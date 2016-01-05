#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

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

    void readSerial();
    void processRawData(double pitch, double volume);


    void on_frequencySlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    static const quint16 arduino_due_verndor_id = 9025;
    static const quint16 arduino_due_product_id = 61;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray serialData;
    QString serialBuffer;

    drumstick::rt::MIDIOutput midiOutput;
    int midichannel;
    int activeNote = -1;
};

#endif // MAINWINDOW_H
