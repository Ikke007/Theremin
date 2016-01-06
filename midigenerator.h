#ifndef MIDIGENERATOR_H
#define MIDIGENERATOR_H

#include <QObject>

#include "midioutput.h"

class MidiGenerator : public QObject
{
    Q_OBJECT
public:
    MidiGenerator();
    void setInvertInput(bool invert);

private:
    bool invertedInput = false;
    drumstick::rt::MIDIOutput midiOutput;
    int midichannel;
    int activeNote = -1;

public slots:
    void processRawInput(double pitch, double volume);

signals:
    void inputGenerated(double frequency, double volume);
};

#endif // MIDIGENERATOR_H
