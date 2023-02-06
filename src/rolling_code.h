#ifndef _RATGDO_ROLLING_CODE_H
#define _RATGDO_ROLLING_CODE_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "BootstrapManager.h"

extern "C" {
#include "secplus.h"
}

void readCounterFromFlash(); // get the rolling code counter from setup.json & return it
void writeCounterToFlash(); // write the counter back to setup.json
void getRollingCode(const char *command); // get the next rolling code for type [reboot1,reboot2,reboot3,reboot4,reboot5,door1,light]
void printRollingCode(byte code[CODE_LENGTH]);

#endif