#include "midigenerator.h"

MidiGenerator::MidiGenerator()
{
    // TODO add midi output selection
        //QStringList connections = midiOutput.connections(true);
        //ui->comboBox->addItems(connections);

    if (!midiOutput.open("CoolSoft VirtualMIDISynth"))
        midiOutput.open("Microsoft GS Wavetable Synth");

    // TODO let the user choose the midi channel

    midiOutput.sendProgram(channel, program); // irgendein Synth Programm

    // eigentlich sollten das hier attack und release time sein,
    // aber da bin ich mir nicht so sicher...
    midiOutput.sendController(channel,72,40);
    midiOutput.sendController(channel,73,0);
    //midiOutput.sendController(midichannel,68,0);
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

    generate();
}

void MidiGenerator::generate() {
    // generate MIDI data
    int noteRange = maxNote - minNote;
    double d_newNote = frequency * noteRange + minNote;
    int newNote = d_newNote;
    double pitch = d_newNote - newNote;

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
    if (activeNote >= 0)
        midiOutput.sendNoteOff(channel, activeNote, 0);
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

