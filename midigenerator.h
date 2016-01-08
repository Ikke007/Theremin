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
    void setMaxNote(int max);
    void setMinNote(int min);
    void stop();
    void restart();
    void generate();

private:
    double frequency;
    double volume;
    bool invertedInput = false;
    drumstick::rt::MIDIOutput midiOutput;
    int channel = 0;
    int program = 50;
    int activeNote = -1;
    int minNote = 0;
    int maxNote = 127;

public slots:
    /** Wertebereich von 0 bis 1 */
    void processRawInput(double pitch, double volume);

signals:
    void inputGenerated(double frequency, double volume);
};

#endif // MIDIGENERATOR_H
