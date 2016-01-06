#ifndef ARDUINOCONTROLLER_H
#define ARDUINOCONTROLLER_H

#include <QSerialPort>

class ArduinoController : public QObject
{
    Q_OBJECT

public:
    ArduinoController();
    ~ArduinoController();

private:
    QSerialPort *arduino;
    static const quint16 arduino_due_verndor_id = 9025;
    static const quint16 arduino_due_product_id = 61;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray serialData;
    QString serialBuffer;

private slots:
    void readSerial();

signals:
    void processRawInput(double frequency, double volume);
};

#endif // ARDUINOCONTROLLER_H
