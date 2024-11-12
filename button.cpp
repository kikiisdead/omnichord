#include "button.h"
#include <Arduino.h>

Button::Button(int buttonPin_) {
  this->buttonPin = buttonPin_;
  buttonState = HIGH;
  lastButtonState = HIGH;
  pinMode(buttonPin, INPUT_PULLUP);
}

int Button::buttonCheck() {
  buttonState = digitalRead(buttonPin);
  int output;
  if (buttonState == LOW && lastButtonState == HIGH) {  // button press inverted HIGH adn LOW because input pullup
    output = 1;
  } else if (buttonState == LOW && lastButtonState == LOW) {  // button sustain
    output = 2;
  } else if (buttonState == HIGH && lastButtonState == LOW) {  // button release
    output = 3;
  } else {
    output = 0;
  }
  lastButtonState = buttonState;
  delay(5);
  return output;
}