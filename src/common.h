#include <Arduino.h>

#define CODE_LENGTH 19 // the length of each command sent to the door.
extern byte txRollingCode[CODE_LENGTH];
extern byte rxRollingCode[CODE_LENGTH];
extern unsigned int rollingCodeCounter;