#ifndef button_h
#define button_h

class Button {
private:
  int buttonPin;
  int buttonState;
  int lastButtonState;
public:
  Button(int buttonPin_);
  int buttonCheck();  //returns an integer based on the result
};

#endif