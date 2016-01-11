#include "midigenerator.h"

#include <QDebug>
#include "math.h"

MidiGenerator::MidiGenerator()
    : running(true),
      invertedInput(false), minNote(0), maxNote(127),
      pitch(0),
      channel(0), program(71), activeNote(-1),
      vibSin(0), vibratoRange(0), vibratoSpeed(0.2)
{
    vibratoTimer.setInterval(10);
    QObject::connect(&vibratoTimer, SIGNAL(timeout()), this, SLOT(sendVibrato()));
}

QStringList MidiGenerator::connections()
{
    return midiOutput.connections(true);
}

bool MidiGenerator::open(QString deviceName)
{
    return midiOutput.open(deviceName);
}

void MidiGenerator::sendVibrato() {
    int pitchbend = (0.5 * pitch + sin(vibSin) * vibratoRange) * 8191;
    midiOutput.sendPitchBend(channel, pitchbend);
    vibSin += vibratoSpeed;
}

void MidiGenerator::processRawInput(double frequency, double volume)
{
    if (invertedInput)
    {
        this->frequency = volume;
        this->volume = frequency;
    } else {
        this->frequency = frequency;
        this->volume = volume;
    }

    if (running)
        generate();
}

void MidiGenerator::generate() {
    // generate MIDI data
    int noteRange = maxNote - minNote;
    double d_newNote = frequency * noteRange + minNote;
    int newNote = d_newNote;
    pitch = d_newNote - newNote;

    if (!vibratoTimer.isActive())
        midiOutput.sendPitchBend(channel, pitch * 4096);

    // fix for MS GS Wavetable Synth
    if (newNote < 0)
        newNote = -1;

    if (activeNote != newNote) {
        midiOutput.sendNoteOn(channel, newNote, 127);
        if (activeNote >= 0)
            midiOutput.sendNoteOff(channel, activeNote, 0);
        activeNote = newNote;
    }

    // send volume
    midiOutput.sendController(channel, 7, volume * 127);

    inputGenerated(frequency, volume);
}

void MidiGenerator::stop() {
    running = false;
    vibratoTimer.stop();
    if (activeNote >= 0)
        midiOutput.sendNoteOff(channel, activeNote, 0);
    activeNote = -1;
}

void MidiGenerator::start() {
    midiOutput.sendProgram(channel, program);

    // Modulation Wheel / Vibrato? -> maximum
    //midiOutput.sendController(channel, 1, 127);

    // eigentlich sollten das hier attack und release time sein,
    midiOutput.sendController(channel,72,0);
    midiOutput.sendController(channel,73,0);
    //midiOutput.sendController(midichannel,68,0);

    running = true;
    vibratoTimer.start();
}

void MidiGenerator::setInvertInput(bool invert) {
    invertedInput = invert;
}

void MidiGenerator::setProgram(int program) {
    this->program = program;
    if (activeNote >= 0)
        midiOutput.sendNoteOff(channel, activeNote, 0);
    midiOutput.sendProgram(channel, program);
    if (activeNote >= 0)
        midiOutput.sendNoteOn(channel, activeNote, 127);
}

void MidiGenerator::setMaxNote(int max) {
    this->maxNote = max;
}

void MidiGenerator::setMinNote(int min) {
    this->minNote = min;
}

void MidiGenerator::setVibratoRange(double range) {
    this->vibratoRange = range;
}

void MidiGenerator::setVibratoSpeed(double speed) {
    this->vibratoSpeed = speed;
}

void MidiGenerator::setChannel(int channel) {
    stop();
    this->channel = channel;
    start();
}
