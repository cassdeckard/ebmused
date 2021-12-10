#ifndef _MIDI_H_

#define _MIDI_H_
void closeMidiInDevice();
void openMidiInDevice(int deviceId, void* callback);

#endif // _MIDI_H_
