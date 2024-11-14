#include "chord.h"
#include "betterEncoder.h"
#include "voice.h"
#include "mixers.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_CAP1188.h>
#include <string.h>

#define CHORDPIN1 33
#define CHORDPIN2 34
#define CHORDPIN3 35
#define CHORDPIN4 36
#define CHORDPIN5 37
#define CHORDPIN6 38
#define CHORDPIN7 39
#define EDITPIN 26

#define ROOTEDIT 0
#define CHORDEDIT 1
#define VOLEDIT 2
#define ANIM 3

#define CAP1188_RESET -1

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_CAP1188 cap = Adafruit_CAP1188();

elapsedMillis holdTime;

Voice sustainVoice1(WAVEFORM_TRIANGLE, 0.5);
Voice sustainVoice2(WAVEFORM_TRIANGLE, 0.5);
Voice sustainVoice3(WAVEFORM_TRIANGLE, 0.5);
Voice strumVoice1(WAVEFORM_TRIANGLE, 0.5);
Voice strumVoice2(WAVEFORM_TRIANGLE, 0.5);
Voice strumVoice3(WAVEFORM_TRIANGLE, 0.5);
Voice strumVoice4(WAVEFORM_TRIANGLE, 0.5);
Voice strumVoice5(WAVEFORM_TRIANGLE, 0.5);
Voice strumVoice6(WAVEFORM_TRIANGLE, 0.5);
Voice strumVoice7(WAVEFORM_TRIANGLE, 0.5);
Voice strumVoice8(WAVEFORM_TRIANGLE, 0.5);

AudioMixer11 mixer;
AudioOutputI2S i2sOut;
AudioControlSGTL5000 audioShield;

AudioConnection patchCord1(sustainVoice1.envelope, 0, mixer, 0);
AudioConnection patchCord2(sustainVoice2.envelope, 0, mixer, 1);
AudioConnection patchCord3(sustainVoice3.envelope, 0, mixer, 2);
AudioConnection patchCord4(strumVoice1.envelope, 0, mixer, 3);
AudioConnection patchCord5(strumVoice2.envelope, 0, mixer, 4);
AudioConnection patchCord6(strumVoice3.envelope, 0, mixer, 5);
AudioConnection patchCord7(strumVoice4.envelope, 0, mixer, 6);
AudioConnection patchCord8(strumVoice5.envelope, 0, mixer, 7);
AudioConnection patchCord9(strumVoice6.envelope, 0, mixer, 8);
AudioConnection patchCord10(strumVoice7.envelope, 0, mixer, 9);
AudioConnection patchCord11(strumVoice8.envelope, 0, mixer, 10);
AudioConnection patchCord12(mixer, 0, i2sOut, 0);
AudioConnection patchCord13(mixer, 0, i2sOut, 1);

Chord chord1(CHORDPIN1);
Chord chord2(CHORDPIN2);
Chord chord3(CHORDPIN3);
Chord chord4(CHORDPIN4);
Chord chord5(CHORDPIN5);
Chord chord6(CHORDPIN6);
Chord chord7(CHORDPIN7);

Voice* sustainVoices[3] = { &sustainVoice1, &sustainVoice2, &sustainVoice3 };
Voice* strumVoices[8] = { &strumVoice1, &strumVoice2, &strumVoice3, &strumVoice4, &strumVoice5, &strumVoice6, &strumVoice7, &strumVoice8 };

Chord* chords[7] = { &chord1, &chord2, &chord3, &chord4, &chord5, &chord6, &chord7 };
Button buttons[7] = { Button(CHORDPIN1), Button(CHORDPIN2), Button(CHORDPIN3), Button(CHORDPIN4), Button(CHORDPIN5), Button(CHORDPIN6), Button(CHORDPIN7) };
Button editButton(EDITPIN);

int editSelector = 0;
int editMode = ROOTEDIT;

float volume = 0.5;

BetterEncoder enc(25, 24, 3);

void setup() {
  Serial.begin(9600);

  enc.incrementHandler(encoderIncrement);
  enc.decrementHandler(encoderDecrement);

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
    chords[i]->initRoot(i * 2);
    chords[i]->setChordType(MAJOR);
  }

  //display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);  // Don't proceed, loop forever
  }
  Serial.println("SSD1306 allocation success!");
  display.clearDisplay();
  displayUI();
  display.display();

  //audio things
  AudioMemory(24);
  audioShield.enable();
  audioShield.volume(volume);

  for (int i = 0; i < 11; i++) {
    mixer.gain(i, 0.5);
  }

  for (int i = 0; i < 3; i ++) {
    sustainVoices[i]->setEnvelope(10, 200, 0.6, 80);
  }
  for (int i = 0; i < 8; i ++) {
    strumVoices[i]->setEnvelope(10, 250, 0.3, 500);
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
  displayUI();
}

void noteOn(int voice, int noteValue, bool selector) {
  if (selector == SUSTAIN) {
    sustainVoices[voice]->noteOn(noteValue);
  } else {
    strumVoices[voice]->noteOn(noteValue);
  }
}

void noteOff(int voice, int noteValue, bool selector) {
  if (selector == SUSTAIN) {
    sustainVoices[voice]->noteOff(noteValue);
  } else {
    strumVoices[voice]->noteOff(noteValue);
  }
}

void encoderIncrement() {
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
    case ANIM:
      break;
  }
}

void encoderDecrement() {
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
    case ANIM:
      break;
  }
}

void checkChordButtons() {
  for (int i = 0; i < 7; i++) {
    if (buttons[i].buttonCheck() == 1) {
      editSelector = i;
    }
  }
}

void checkEdit() {
  if (editButton.buttonCheck() == 1) {
    editMode += 1;
    if (editMode > 2) {
      editMode = 0;
    }
    holdTime = 0;
  } else if (editButton.buttonCheck() == 2) {
    if (holdTime >= 500) {
      editMode = ANIM;
    }
  }
}

void displayUI() {
  int root = chords[editSelector]->getRoot();
  chordTypes type = chords[editSelector]->getChordType();
  display.clearDisplay();
  
  display.setTextColor(WHITE);

  if (editMode < 3) {
    display.setCursor(0, 0);
    displayChordRoot(root);
    display.setCursor(0, 32);
    displayChordType(type);
    display.setCursor(80, 0);
    displayVolume();
  } else {
    animateStrings();
  }
  display.display();
}

void animateStrings() {
  for (int i = 0; i < 8; i ++) {
    int yLevel = (i*8) + 4;
    float jitterAmount = strumVoices[i]->readPeak() * 4;
    float prevJitter = 0;
    for (int j = 0; j < display.width(); j += 8) {
      int randomInt = random(-4, 5);
      float jitter = randomInt * jitterAmount;
      display.drawLine(j, yLevel + prevJitter, j+8, yLevel + jitter, WHITE);
      prevJitter = jitter;
    }
  }
}

void displayVolume() {
  display.setTextSize(1); 
  display.println("Volume:");
  display.setCursor(68, 10);
  display.setTextSize(2); 
  if (editMode == VOLEDIT) {
    display.print(F(">"));
  } else {
    display.print(F(" "));
  }
  String volStr = String(volume, 2);
  display.println(volStr);
  
}

void displayChordRoot(int root) {
  display.setTextSize(1); 
  display.println("Chord Root:");
  display.setCursor(0, 10);
  display.setTextSize(2); 
  switch (abs(root % 12)) {
    case 0:
      display.print(F("C"));
      break;
    case 1:
      display.print(F("C#/Db"));
      break;
    case 2:
      display.print(F("D"));
      break;
    case 3:
      display.print(F("D#/Eb"));
      break;
    case 4:
      display.print(F("E"));
      break;
    case 5:
      display.print(F("F"));
      break;
    case 6:
      display.print(F("F#/Gb"));
      break;
    case 7:
      display.print(F("G"));
      break;
    case 8:
      display.print(F("G#/Ab"));
      break;
    case 9:
      display.print(F("A"));
      break;
    case 10:
      display.print(F("A#/Bb"));
      break;
    case 11:
      display.print(F("B"));
      break;
  }
  if (editMode == ROOTEDIT) {
    display.println(F("<"));
  } else {
    display.println(F(""));
  }
}

void displayChordType(chordTypes type) {
  display.setTextSize(1); 
  display.println("Chord Type:");
  display.setCursor(0, 42);
  display.setTextSize(2); 
  switch (type) {
    case MAJOR:
      display.print(F("Maj"));
      break;
    case MINOR:
      display.print(F("min"));
      break;
    case AUGMENTED:
      display.print(F("aug"));
      break;
    case DIMINISHED:
      display.print(F("dim"));
      break;
    case MAJORSEVEN:
      display.print(F("Maj7"));
      break;
    case MINORSEVEN:
      display.print(F("min7"));
      break;
    case DOMINANTSEVEN:
      display.print(F("dom7"));
      break;
    case HALFDIMINISHEDSEVEN:
      display.print(F("m7b5"));
      break;
    case FULLDIMINISHEDSEVEN:
      display.print(F("dim7"));
      break;
  }
  if (editMode == CHORDEDIT) {
    display.println("<");
  } else {
    display.println(F(""));
  }
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