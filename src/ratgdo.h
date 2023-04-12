/************************************
 * Rage
 * Against
 * The
 * Garage
 * Door
 * Opener
 *
 * Copyright (C) 2022  Paul Wieland
 *
 * GNU GENERAL PUBLIC LICENSE
 ************************************/

#ifndef _RATGDO_H
#define _RATGDO_H


#include "BootstrapManager.h" // Must use the https://github.com/PaulWieland/arduinoImprovBootstrapper fork, ratgdo branch
#include "SoftwareSerial.h" // Using espsoftwareserial https://github.com/plerup/espsoftwareserial
#include "rolling_code.h"
#include "home_assistant.h"

SoftwareSerial swSerial;

/********************************** BOOTSTRAP MANAGER *****************************************/
BootstrapManager bootstrapManager;

/********************************** PIN DEFINITIONS *****************************************/
#define INPUT_GDO D2 // 
#define OUTPUT_GDO D4 // red control terminal / GarageDoorOpener (UART1 TX) pin is D4 on D1 Mini
#define TRIGGER_OPEN D5 // dry contact for opening door
#define TRIGGER_CLOSE D6 // dry contact for closing door
#define TRIGGER_LIGHT D3 // dry contact for triggering light (no discrete light commands, so toggle only)
#define STATUS_DOOR D0 // output door status, HIGH for open, LOW for closed
#define STATUS_OBST D8 // output for obstruction status, HIGH for obstructed, LOW for clear
#define INPUT_OBST D7 // black obstruction sensor terminal


/********************************** MQTT TOPICS & Statuses *****************************************/
String availabilityStatusTopic = ""; // will be mqttTopicPrefix/deviceName/status/availability
// online|offline

String commandTopic = "";     // will be mqttTopicPrefix/deviceName/command/#

String doorCommandTopic = ""; // will be mqttTopicPrefix/deviceName/command/door
                              // accepts [open|close|stop]
String doorStatusTopic = "";  // will be mqttTopicPrefix/deviceName/status/door
uint8_t doorState = 0;
String doorStates[6] = {"unknown","open","closed","stopped","opening","closing"};


String lightCommandTopic = "";// will be mqttTopicPrefix/deviceName/command/light
                              // accepts [on|off]
String lightStatusTopic = ""; // will be mqttTopicPrefix/deviceName/status/light
uint8_t lightState = 2;
String lightStates[3] = {"off","on","unknown"};


String lockCommandTopic = ""; // will be mqttTopicPrefix/deviceName/command/lock
                              // accepts [lock|unlock]
String lockStatusTopic = "";  // will be mqttTopicPrefix/deviceName/status/lock
uint8_t lockState = 2;
String lockStates[3] = {"unlocked","locked","unknown"};

String obstructionStatusTopic = ""; // will be mqttTopicPrefix/deviceName/status/obstruction
uint8_t obstructionState = 2;
String obstructionStates[3] = {"obstructed","clear","unknown"};

String setCounterTopic = "";  // will be mqttTopicPrefix/deviceName/set_code_counter
String rollingCodeTopic = ""; // broadcast the current rolling code count for debugging purposes

/********************************** GLOBAL VARS *****************************************/
bool setupComplete = false;
unsigned int rollingCodeCounter;
byte txRollingCode[CODE_LENGTH];
byte rxRollingCode[CODE_LENGTH];

unsigned int obstructionLowCount = 0;  // count obstruction low pulses
unsigned long lastObstructionHigh = 0;  // count time between high pulses from the obst ISR


bool dryContactDoorOpen = false;
bool dryContactDoorClose = false;
bool dryContactToggleLight = false;

/********************************** FUNCTION DECLARATION *****************************************/
void callback(char *topic, byte *payload, unsigned int length);
void manageDisconnections();
void manageQueueSubscription();
void manageHardwareButton();

void transmit(byte* payload, unsigned int length);
void sync();

void toggleDoor();
void openDoor();
void closeDoor();
void stopDoor();
void sendDoorStatus();

void toggleLight();
void lightOn();
void lightOff();
void sendLightStatus();

void toggleLock();
void lock();
void unlock();
void sendLockStatus();

void obstructionLoop();
void sendObstructionStatus();

void statusUpdateLoop();

void gdoStateLoop();
void dryContactLoop();

/********************************** INTERRUPT SERVICE ROUTINES ***********************************/
void IRAM_ATTR isrDebounce(const char *type);
void IRAM_ATTR isrDoorOpen();
void IRAM_ATTR isrDoorClose();
void IRAM_ATTR isrLight();
void IRAM_ATTR isrObstruction();
void IRAM_ATTR isrRPM1();
void IRAM_ATTR isrRPM2();

#endif