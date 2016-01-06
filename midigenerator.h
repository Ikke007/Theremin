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
    void setProgram(int program);

private:
    bool invertedInput = false;
    drumstick::rt::MIDIOutput midiOutput;
    int channel = 0;
    int program = 50;
    int activeNote = -1;

public slots:
    /** Wertebereich von 0 bis 1 */
    void processRawInput(double pitch, double volume);

signals:
    void inputGenerated(double frequency, double volume);
};

#endif // MIDIGENERATOR_H
