#ifndef MIDIGENERATOR_H
#define MIDIGENERATOR_H

#include <QObject>
#include <QTimer>

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
    void start();
    void generate();
    /* 0(min) - 1(max) */
    void setVibratoRange(double range);
    /* 0(min) - 1(max) */
    void setVibratoSpeed(double speed);

private:
    double frequency;
    double volume;
    bool invertedInput = false;
    drumstick::rt::MIDIOutput midiOutput;
    int channel = 0;
    int program = 50;
    int activeNote = -1;
    double pitch = 0;
    int minNote = 0;
    int maxNote = 127;

    bool running = true;

    double vibSin = 0;
    double vibratoRange = 0;
    double vibratoSpeed = 2;
    QTimer vibratoTimer;

public slots:
    /** Wertebereich von 0 bis 1 */
    void processRawInput(double pitch, double volume);
    void sendVibrato();

signals:
    void inputGenerated(double frequency, double volume);
};

#endif // MIDIGENERATOR_H
