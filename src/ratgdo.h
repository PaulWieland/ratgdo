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


#include "BootstrapManager.h" // Must use the https://github.com/PaulWieland/arduinoImprovBootstrapper fork, ratgdo branch
#include "SoftwareSerial.h" // Using espsoftwareserial https://github.com/plerup/espsoftwareserial
#include "rolling_code.h"

SoftwareSerial swSerial;

/********************************** BOOTSTRAP MANAGER *****************************************/
BootstrapManager bootstrapManager;

/********************************** PIN DEFINITIONS *****************************************/
#define OUTPUT_GDO D4 // red control terminal / GarageDoorOpener (UART1 TX) pin is D4 on D1 Mini
#define TRIGGER_OPEN D5 // dry contact for opening door
#define TRIGGER_CLOSE D6 // dry contact for closing door
#define TRIGGER_LIGHT D3 // dry contact for triggering light (no discrete light commands, so toggle only)
#define STATUS_DOOR D0 // output door status, HIGH for open, LOW for closed
#define STATUS_OBST D8 // output for obstruction status, HIGH for obstructed, LOW for clear
#define INPUT_RPM1 D1 // RPM1 rotary encoder input OR reed switch if not soldering to the door opener logic board
#define INPUT_RPM2 D2 // RPM2 rotary encoder input OR not used if using reed switch
#define INPUT_OBST D7 // black obstruction sensor terminal


/********************************** MQTT TOPICS *****************************************/
String doorCommandTopic = ""; // will be mqttTopicPrefix/deviceName/command
String setCounterTopic = ""; // will be mqttTopicPrefix/deviceName/set_code_counter

String doorCommand = "";      // will be [open|close|light]
String overallStatusTopic = ""; // legacy from 1.0. Will be mqttTopicPrefix/deviceName/status
String availabilityStatusTopic = ""; // online|offline
String obstructionStatusTopic = ""; // obstructed|clear
String doorStatusTopic = ""; // open|opening|closing|closed|reed_open|reed_closed
String rollingCodeTopic = ""; // broadcast the current rolling code count for debugging purposes

/********************************** GLOBAL VARS *****************************************/
bool setupComplete = false;
unsigned int rollingCodeCounter;
byte rollingCode[CODE_LENGTH];
String doorState = "unknown";        // will be [online|offline|opening|open|closing|closed|obstructed|clear|reed_open|reed_closed]

unsigned int obstructionLowCount = 0;  // count obstruction low pulses
unsigned long lastObstructionHigh = 0;  // count time between high pulses from the obst ISR

bool doorIsObstructed = false;
bool dryContactDoorOpen = false;
bool dryContactDoorClose = false;
bool dryContactToggleLight = false;
int doorPositionCounter = 0;         // calculate the door's movement and position
bool rpm1Pulsed = false;             // did rpm1 get a pulse or not - eliminates an issue when the sensor is parked on a high pulse which fires rpm2 isr

/********************************** FUNCTION DECLARATION *****************************************/
void callback(char *topic, byte *payload, unsigned int length);
void manageDisconnections();
void manageQueueSubscription();
void manageHardwareButton();

void transmit(byte* payload, unsigned int length);
void sync();
void openDoor();
void closeDoor();
void toggleLight();

void obstructionLoop();
void obstructionDetected();
void obstructionCleared();

void sendDoorStatus();

void doorStateLoop();
void dryContactLoop();

/********************************** INTERRUPT SERVICE ROUTINES ***********************************/
void IRAM_ATTR isrDebounce(const char *type);
void IRAM_ATTR isrDoorOpen();
void IRAM_ATTR isrDoorClose();
void IRAM_ATTR isrLight();
void IRAM_ATTR isrObstruction();
void IRAM_ATTR isrRPM1();
void IRAM_ATTR isrRPM2();

/*** Static Codes ***/
byte SYNC1[] = {0x55,0x01,0x00,0x61,0x12,0x49,0x2c,0x92,0x5b,0x24,0x96,0x86,0x0b,0x65,0x96,0xd9,0x8f,0x26,0x4a};
byte SYNC2[] = {0x55,0x01,0x00,0x08,0x34,0x93,0x49,0xb4,0x92,0x4d,0x20,0x26,0x1b,0x4d,0xb4,0xdb,0xad,0x76,0x93};
byte SYNC3[] = {0x55,0x01,0x00,0x06,0x1b,0x2c,0xbf,0x4b,0x6d,0xb6,0x4b,0x18,0x20,0x92,0x09,0x20,0xf2,0x11,0x2c};
byte SYNC4[] = {0x55,0x01,0x00,0x95,0x29,0x36,0x91,0x29,0x36,0x9a,0x69,0x05,0x2f,0xbe,0xdf,0x6d,0x16,0xcb,0xe7};
byte* SYNC_CODE[] = {SYNC1,SYNC2,SYNC3,SYNC4};

byte DOOR_CODE[] = {0x55,0x01,0x00,0x94,0x3f,0xef,0xbc,0xfb,0x7f,0xbe,0xfc,0xa6,0x1a,0x4d,0xa6,0xda,0x8d,0x36,0xb3};

byte LIGHT_CODE[] = {0x55,0x01,0x00,0x94,0x3f,0xef,0xbc,0xfb,0x7f,0xbe,0xff,0xa6,0x1a,0x4d,0xa6,0xda,0x8d,0x76,0xb1};