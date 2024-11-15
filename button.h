#ifndef button_h
#define button_h

#include <Arduino.h>

class Button {
private:
  int buttonPin;
  int buttonState;
  int lastButtonState;
  elapsedMillis timeSinceEvent;
public:
  Button(int buttonPin_);
  int buttonCheck();  //returns an integer based on the result
};

#endif