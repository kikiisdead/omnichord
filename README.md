# MINICHORD
 a mini omnichord made using Teensy 4.1 and the teensy audio library in Arduino

TODO
 - see if theres a way to simplify my chord object because she is complicated
 - see if I can simplify anything else
  - simplify the display functions? it might be helpful to move into object?
  - maybe doing this so that it can be made into a bit map?
 - Might be able to make the synthesis more complex?
  - need to reduce the amount of strum voices to take load off of teensy first (need to implement some form of polyphony logic)

COMPLETED
 - figured out a way to wrap an oscillator and envelope into a single object.