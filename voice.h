#ifndef voice_h
#define voice_h

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

class Voice {
public:
  AudioSynthWaveform              waveform;
  AudioEffectEnvelope             envelope;
  AudioConnection                 *patchCord[1]; // total patchCordCount:1 including array typed ones.

  Voice(int waveform, float amplitude);
  void setEnvelope(float attack, float decay, float sustain, float release);
  void noteOn(int note);
  void noteOff();
};

#endif 