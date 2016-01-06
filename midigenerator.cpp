#include "midigenerator.h"

MidiGenerator::MidiGenerator()
{
    // TODO add midi output selection
        //QStringList connections = midiOutput.connections(true);
        //ui->comboBox->addItems(connections);

    if (!midiOutput.open("CoolSoft VirtualMIDISynth"))
        midiOutput.open("Microsoft GS Wavetable Synth");

    // TODO let the user choose the midi channel
    midichannel = 0;
    midiOutput.sendProgram(midichannel, 81); // irgendein Synth Programm

    // eigentlich sollten das hier attack und release time sein,
    // aber da bin ich mir nicht so sicher...
    midiOutput.sendController(midichannel,72,40);
    midiOutput.sendController(midichannel,73,0);
    //midiOutput.sendController(midichannel,68,0);
}

// TODO auf irgendeinen Wertebereich festlegen? Zum testen hab ich 0 bis 100 genommen, das ist wohl ein bisschen klein ;)
// von 0 bis 1 ?
void MidiGenerator::processRawInput(double frequency, double volume)
{
    if (invertedInput)
    {
        double temp = frequency;
        frequency = volume;
        volume = temp;
    }

    // generate MIDI data
    int newNote = frequency * 127;
    double pitch = frequency * 127 - newNote;

    midiOutput.sendPitchBend(midichannel, pitch * 4096);

    // fix for MS GS Wavetable Synth
    if (newNote < 0)
        newNote = -1;

    //midiOutput.sendPitchBend(midichannel, volume * 8192);
    if (activeNote != newNote) {
        midiOutput.sendNoteOn(midichannel, newNote, 127);
        midiOutput.sendNoteOff(midichannel, activeNote, 0);
        activeNote = newNote;
    }

    // send volume
    midiOutput.sendController(midichannel, 7, volume * 127);

    inputGenerated(frequency, volume);
}

void MidiGenerator::setInvertInput(bool invert) {
    invertedInput = invert;
}

