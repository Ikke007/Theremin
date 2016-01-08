#include "midigenerator.h"

#include <QDebug>

MidiGenerator::MidiGenerator()
{
    // TODO add midi output selection
        //QStringList connections = midiOutput.connections(true);
        //ui->comboBox->addItems(connections);

    if (!midiOutput.open("CoolSoft VirtualMIDISynth"))
        midiOutput.open("Microsoft GS Wavetable Synth");

    // TODO let the user choose the midi channel
    midiOutput.sendProgram(channel, program); // irgendein Synth Programm

    // Modulation Wheel / Vibrato? -> maximum
    //midiOutput.sendController(channel, 1, 127);
    // TODO make modWheel adjustable

    // eigentlich sollten das hier attack und release time sein,
    // aber da bin ich mir nicht so sicher...
    midiOutput.sendController(channel,72,40);
    midiOutput.sendController(channel,73,0);
    //midiOutput.sendController(midichannel,68,0);

    vibratoTimer.setInterval(10);
    QObject::connect(&vibratoTimer, SIGNAL(timeout()), this, SLOT(sendVibrato()));
    vibratoTimer.start();
}

void MidiGenerator::sendVibrato() {
    int pitchbend = (pitch + sin(vibSin) * vibratoRange) * 4096;
    qDebug() << "pitchbend: " << pitchbend;
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
    qDebug() << "set pitch: " << pitch;

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
}

void MidiGenerator::start() {
    // TODO move contructor initialisations here
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

