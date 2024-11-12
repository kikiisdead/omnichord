#include "chord.h"
#include "betterEncoder.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_CAP1188.h>

#define chordPin1 25
#define chordPin2 26
#define chordPin3 27
#define chordPin4 28
#define chordPin5 29
#define chordPin6 30
#define chordPin7 31
#define editPin 10

#define ROOTEDIT 0
#define CHORDEDIT 1
#define VOLEDIT 2

#define CAP1188_RESET -1

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_CAP1188 cap = Adafruit_CAP1188();

static const unsigned char PROGMEM logo_bmp[] = { 0b00000000, 0b11000000, 0b00000001, 0b11000000, 0b00000001, 0b11000000, 0b00000011, 0b11100000, 0b11110011, 0b11100000, 0b11111110, 0b11111000, 0b01111110, 0b11111111, 0b00110011, 0b10011111, 0b00011111, 0b11111100, 0b00001101, 0b01110000, 0b00011011, 0b10100000, 0b00111111, 0b11100000, 0b00111111, 0b11110000, 0b01111100, 0b11110000, 0b01110000, 0b01110000, 0b00000000, 0b00110000 };

// GUItool: begin automatically generated code
  AudioSynthWaveform       sustainOsc1;    //xy=655.5,66
  AudioSynthWaveform       sustainOsc2;    //xy=656.5,155
  AudioSynthWaveform       sustainOsc3;    //xy=659.5,245
  AudioSynthWaveform       strumOsc8;      //xy=664.5,1041
  AudioSynthWaveform       strumOsc6;      //xy=667.5,860
  AudioSynthWaveform       strumOsc7;      //xy=669.5,946
  AudioSynthWaveform       strumOsc4;      //xy=672.5,657
  AudioSynthWaveform       strumOsc1;      //xy=675.5,349
  AudioSynthWaveform       strumOsc2;      //xy=677.5,467
  AudioSynthWaveform       strumOsc3;      //xy=678.5,573
  AudioSynthWaveform       strumOsc5;      //xy=684.5,751
  AudioEffectEnvelope      sustainEnv1;    //xy=984.7500114440918,91.25000190734863
  AudioEffectEnvelope      sustainEnv3;    //xy=985.7500114440918,230.25000190734863
  AudioEffectEnvelope      strumEnv8;      //xy=982.5,1066
  AudioEffectEnvelope      strumEnv6;      //xy=983.5,868
  AudioEffectEnvelope      strumEnv5;      //xy=985.5,768
  AudioEffectEnvelope      strumEnv4;      //xy=986.5,652
  AudioEffectEnvelope      strumEnv3;      //xy=988.5,567
  AudioEffectEnvelope      strumEnv2;      //xy=991.5,466
  AudioEffectEnvelope      sustainEnv2;    //xy=992.7500114440918,168.25000190734863
  AudioEffectEnvelope      strumEnv1;      //xy=993.5,346
  AudioEffectEnvelope      strumEnv7;      //xy=991.5,972
  AudioMixer4              sustainMixer;   //xy=1192.000015258789,175.00000381469727
  AudioMixer4              strumMixer1;    //xy=1210.5,489
  AudioMixer4              strumMixer2;    //xy=1216.5,917
  AudioMixer4              finalMixer;     //xy=1381.5,495
  AudioOutputI2S           i2sOut;         //xy=1552.5,493
  AudioConnection          patchCord1(sustainOsc1, sustainEnv1);
  AudioConnection          patchCord2(sustainOsc2, sustainEnv2);
  AudioConnection          patchCord3(sustainOsc3, sustainEnv3);
  AudioConnection          patchCord4(strumOsc8, strumEnv8);
  AudioConnection          patchCord5(strumOsc6, strumEnv6);
  AudioConnection          patchCord6(strumOsc7, strumEnv7);
  AudioConnection          patchCord7(strumOsc4, strumEnv4);
  AudioConnection          patchCord8(strumOsc1, strumEnv1);
  AudioConnection          patchCord9(strumOsc2, strumEnv2);
  AudioConnection          patchCord10(strumOsc3, strumEnv3);
  AudioConnection          patchCord11(strumOsc5, strumEnv5);
  AudioConnection          patchCord12(sustainEnv1, 0, sustainMixer, 0);
  AudioConnection          patchCord13(sustainEnv3, 0, sustainMixer, 2);
  AudioConnection          patchCord14(strumEnv8, 0, strumMixer2, 3);
  AudioConnection          patchCord15(strumEnv6, 0, strumMixer2, 1);
  AudioConnection          patchCord16(strumEnv5, 0, strumMixer2, 0);
  AudioConnection          patchCord17(strumEnv4, 0, strumMixer1, 3);
  AudioConnection          patchCord18(strumEnv3, 0, strumMixer1, 2);
  AudioConnection          patchCord19(strumEnv2, 0, strumMixer1, 1);
  AudioConnection          patchCord20(sustainEnv2, 0, sustainMixer, 1);
  AudioConnection          patchCord21(strumEnv1, 0, strumMixer1, 0);
  AudioConnection          patchCord22(strumEnv7, 0, strumMixer2, 2);
  AudioConnection          patchCord23(sustainMixer, 0, finalMixer, 0);
  AudioConnection          patchCord24(strumMixer1, 0, finalMixer, 1);
  AudioConnection          patchCord25(strumMixer2, 0, finalMixer, 2);
  AudioConnection          patchCord26(finalMixer, 0, i2sOut, 0);
  AudioConnection          patchCord27(finalMixer, 0, i2sOut, 1);
  AudioControlSGTL5000     audioShield;    //xy=1003.5,50
// GUItool: end automatically generated code

Chord chord1(chordPin1);
Chord chord2(chordPin2);
Chord chord3(chordPin3);
Chord chord4(chordPin4);
Chord chord5(chordPin5);
Chord chord6(chordPin6);
Chord chord7(chordPin7);

//sustain voices parallel lists of things
AudioSynthWaveform* sustainOscs[3] = { &sustainOsc1, &sustainOsc2, &sustainOsc3 };
AudioEffectEnvelope* sustainEnvs[3] = { &sustainEnv1, &sustainEnv2, &sustainEnv3 };

//strum voices parallel list of things
AudioSynthWaveform* strumOscs[8] = { &strumOsc1, &strumOsc2, &strumOsc3, &strumOsc4, &strumOsc5, &strumOsc6, &strumOsc7, &strumOsc8 };
AudioEffectEnvelope* strumEnvs[8] = { &strumEnv2, &strumEnv2, &strumEnv3, &strumEnv4, &strumEnv5, &strumEnv6, &strumEnv7, &strumEnv8 };

AudioMixer4* mixers[4] = { &sustainMixer, &strumMixer1, &strumMixer2, &finalMixer };

Chord* chords[7] = { &chord1, &chord2, &chord3, &chord4, &chord5, &chord6, &chord7 };
Button buttons[7] = { Button(chordPin1), Button(chordPin2), Button(chordPin3), Button(chordPin4), Button(chordPin5), Button(chordPin6), Button(chordPin7) };
Button editButton(editPin);

int editSelector = 0;
int editMode = ROOTEDIT;

float volume = 0.5;

BetterEncoder enc(11, 12, 4);

void setup() {
  Serial.begin(9600);

  enc.incrementHandler(oneClickRight);
  enc.decrementHandler(oneClickLeft);

  //capacitive touch sensor
  if (!cap.begin()) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");

  //chord objects
  for (int i = 0; i < 7; i++) {
    chords[i]->noteOnHandler(noteOn);
    chords[i]->noteOffHandler(noteOff);
    chords[i]->capCheckHandler(adaCapCheck);
    chords[i]->initRoot(i*2);
    chords[i]->setChordType(MAJOR);
  }

  //display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);  // Don't proceed, loop forever
  }
  Serial.println("SSD1306 allocation success!");
  display.clearDisplay();
  displayChordRoot(chords[editSelector]->getRoot(), chords[editSelector]->getChordType());
  display.display();

  //audio things
  AudioMemory(24);
  audioShield.enable();
  audioShield.volume(volume);
  //initializing all the audio objects
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mixers[j]->gain(i, 1);
    }
  }
  for (int i = 0; i < 3; i++) {
    sustainOscs[i]->begin(WAVEFORM_TRIANGLE);
    sustainOscs[i]->amplitude(0.5);
  }
  for (int i = 0; i < 8; i++) {
    strumOscs[i]->begin(WAVEFORM_TRIANGLE);
    strumOscs[i]->amplitude(0.4);
    strumEnvs[i]->release(1000);
  }
  //everything works if it hits this point
  Serial.println("I'm working");
}

void loop() {
  enc.process();
  checkEdit();
  checkChordButtons();
  for (int i = 0; i < 7; i++) {
    chords[i]->update();
  }
}

void noteOn(int voice, int noteValue, bool selector) {
  usbMIDI.sendNoteOn(noteValue, 127, 1);
  float freq = 440 * pow(2.0, (float)(noteValue - 69) / (float)12);  //turn the midi note value into frequency value to be used with the oscillator
  if (selector == SUSTAIN) {
    sustainOscs[voice]->frequency(freq);
    sustainEnvs[voice]->noteOn();
  } else {
    strumOscs[voice]->frequency(freq);
    strumEnvs[voice]->noteOn();
  }
}

void noteOff(int voice, int noteValue, bool selector) {
  usbMIDI.sendNoteOff(noteValue, 0, 1);
  if (selector == SUSTAIN) {
    sustainEnvs[voice]->noteOff();
  } else {
    strumEnvs[voice]->noteOff();
  }
}

void oneClickLeft() {
  // increment our count variable and print out that number
  int chordRoot = chords[editSelector]->getRoot();
  int chordType = ((int)chords[editSelector]->getChordType() == 8) ? 0 : (int)chords[editSelector]->getChordType() + 1;
  switch (editMode) {
    case ROOTEDIT:
      chords[editSelector]->setRoot(chordRoot + 1);
      break;
    case CHORDEDIT:
      chords[editSelector]->setChordType((chordTypes)chordType);
      break;
    case VOLEDIT:
      volumeIncrement();
      audioShield.volume(volume);
      break;
  }
  displayChordRoot(chords[editSelector]->getRoot(), chords[editSelector]->getChordType());
}

void oneClickRight() {
  // decrement our count variable and print out that number
  int chordRoot = chords[editSelector]->getRoot();
  int chordType = ((int)chords[editSelector]->getChordType() == 0) ? 8 : (int)chords[editSelector]->getChordType() - 1;
  switch (editMode) {
    case ROOTEDIT:
      chords[editSelector]->setRoot(chordRoot - 1);
      break;
    case CHORDEDIT:
      chords[editSelector]->setChordType((chordTypes)chordType);
      break;
    case VOLEDIT:
      volumeDecrement();
      audioShield.volume(volume);
      break;
  }
  displayChordRoot(chords[editSelector]->getRoot(), chords[editSelector]->getChordType());
}

void checkChordButtons() {
  for (int i = 0; i < 7; i++) {
    if (buttons[i].buttonCheck() == 1) {
      editSelector = i;
      displayChordRoot(chords[editSelector]->getRoot(), chords[editSelector]->getChordType());
    }
  }
}

void checkEdit() {
  if (editButton.buttonCheck() == 1) {
    editMode += 1;
    if (editMode > 2) {
      editMode = 0;
    }
  }
  displayChordRoot(chords[editSelector]->getRoot(), chords[editSelector]->getChordType());
}

void displayChordRoot(int root, chordTypes type) {
  display.clearDisplay();
  display.setTextSize(4);               // Normal 1:1 pixel scale
  display.setCursor(0, 0);

  if (editMode == ROOTEDIT) {
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //highlight if in ROOTEDIT
  } else {
    display.setTextColor(SSD1306_WHITE);
  }
  switch (abs(root % 12)) {
    case 0:
      display.println(F("C"));
      break;
    case 1:
      display.println(F("C#/Db"));
      break;
    case 2:
      display.println(F("D"));
      break;
    case 3:
      display.println(F("D#/Eb"));
      break;
    case 4:
      display.println(F("E"));
      break;
    case 5:
      display.println(F("F"));
      break;
    case 6:
      display.println(F("F#/Gb"));
      break;
    case 7:
      display.println(F("G"));
      break;
    case 8:
      display.println(F("G#/Ab"));
      break;
    case 9:
      display.println(F("A"));
      break;
    case 10:
      display.println(F("A#/Bb"));
      break;
    case 11:
      display.println(F("B"));
      break;
  }

  if (editMode == CHORDEDIT) {
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //highlight if in CHORDEDIT
  } else {
    display.setTextColor(SSD1306_WHITE);
  }
  switch (type) {
    case MAJOR:
      display.println(F("Maj"));
      break;
    case MINOR:
      display.println(F("min"));
      break;
    case AUGMENTED:
      display.println(F("aug"));
      break;
    case DIMINISHED:
      display.println(F("dim"));
      break;
    case MAJORSEVEN:
      display.println(F("Maj7"));
      break;
    case MINORSEVEN:
      display.println(F("min7"));
      break;
    case DOMINANTSEVEN:
      display.println(F("dom7"));
      break;
    case HALFDIMINISHEDSEVEN:
      display.println(F("m7b5"));
      break;
    case FULLDIMINISHEDSEVEN:
      display.println(F("dim7"));
      break;
  }

  display.display();
}

void adaCapCheck(int noteTouch[8]) {
  uint8_t touched = cap.touched();
  for (uint8_t i = 0; i < 8; i++) {
    if (touched & (1 << i)) {
      noteTouch[i] = HIGH;  //simply returning the position that it is touched/which cap is being touched. returns whichever is firt so only one touch position;
    } else {
      noteTouch[i] = LOW;
    }
  }
}

void volumeIncrement() {
  if (volume < 1) {
    volume += 0.01;
  }
}

void volumeDecrement() {
  if (volume > 0) {
    volume -= 0.01;
  }
}