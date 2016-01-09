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
    bool invertedInput;
    drumstick::rt::MIDIOutput midiOutput;
    int channel;
    int program;
    int activeNote;
    double pitch;
    int minNote;
    int maxNote;

    bool running;

    double vibSin;
    double vibratoRange;
    double vibratoSpeed;
    QTimer vibratoTimer;

public slots:
    /** Wertebereich von 0 bis 1 */
    void processRawInput(double pitch, double volume);
    void sendVibrato();

signals:
    void inputGenerated(double frequency, double volume);
};

#endif // MIDIGENERATOR_H
