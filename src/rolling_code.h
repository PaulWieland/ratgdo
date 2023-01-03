#ifndef _RATGDO_ROLLING_CODE_H
#define _RATGDO_ROLLING_CODE_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "BootstrapManager.h"

#define CODE_SET_LENGTH 46 // the length of each group of commands per counter. 16 bytes for reboot1 & 6 bytes each for 5 remaining codes
#define BIN_COUNT_OFFSET 268371455 // the starting counter used when generating codes.hex

void readCounterFromFlash(); // get the rolling code counter from setup.json & return it
void writeCounterToFlash(); // write the counter back to setup.json
void getRollingCode(const char *command); // get the next rolling code for type [reboot1,reboot2,reboot3,reboot4,reboot5,door1,light]
void printRollingCode();

#endif