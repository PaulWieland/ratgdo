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
#define TRIGGER_LIGHT D3 // D7 on Rev0 dry contact for triggering light (no discrete light commands, so toggle only)
#define STATUS_DOOR D0 // output door status, HIGH for open, LOW for closed
#define STATUS_OBST D8 // output for obstruction status, HIGH for obstructed, LOW for clear
#define INPUT_RPM1 D1 // RPM1 rotary encoder input OR reed switch if not soldering to the door opener logic board
#define INPUT_RPM2 D2 // RPM2 rotary encoder input OR not used if using reed switch
#define INPUT_OBST D7 // D3 on Rev0 black obstruction sensor terminal


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
unsigned long obstructionTimer = 0;  // count time between pulses from the obst ISR
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
 byte DOOR1[] = {0x55,0x01,0x00,0xa1,0x13,0x4d,0x24,0xd3,0x69,0xa4,0x97,0x4a,0x2d,0xbe,0xdf,0x6f,0x1e,0xcf,0x7d};
 byte DOOR2[] = {0x55,0x01,0x00,0x04,0x3f,0xf7,0x9b,0xef,0xff,0xff,0x7f,0xaa,0x2c,0x96,0xd9,0x6d,0xc4,0x5d,0x73};
 byte DOOR3[] = {0x55,0x01,0x00,0x46,0x0b,0x6d,0x92,0xcb,0x65,0xb6,0x4b,0xa0,0x3e,0xdb,0x6f,0xb7,0x39,0xad,0xb2};
 byte DOOR4[] = {0x55,0x01,0x00,0x91,0x16,0x49,0x26,0xd2,0xc9,0x64,0x96,0x49,0x02,0x41,0x00,0x00,0xa8,0x44,0x22};
 byte DOOR5[] = {0x55,0x01,0x00,0x90,0x16,0x5b,0x6f,0xf6,0x5b,0x2d,0xb6,0x2a,0x25,0xb6,0x5b,0x6c,0x76,0x79,0x75};
 byte DOOR6[] = {0x55,0x01,0x00,0x26,0x1a,0x4d,0xae,0x93,0x6d,0x36,0x95,0x9a,0x2d,0xa4,0xdb,0x6d,0x56,0x93,0x45};
 byte* DOOR_CODE[] = {DOOR1,DOOR2,DOOR3,DOOR4,DOOR5,DOOR6};

 byte LIGHT1[] = {0x55,0x01,0x00,0x11,0x16,0xc9,0x25,0x92,0x49,0x64,0x94,0x10,0x32,0xdb,0x2c,0xb6,0xbb,0x0d,0xb6};
 byte LIGHT2[] = {0x55,0x01,0x00,0x10,0x36,0xcb,0x4c,0xb6,0xdb,0x2d,0xb6,0x01,0x10,0x41,0x00,0x90,0x7b,0x26,0x9d};
 byte LIGHT3[] = {0x55,0x01,0x00,0x66,0x3b,0x7d,0xfe,0xdf,0x7f,0xb7,0xfb,0x60,0x16,0x5b,0x2c,0xb6,0xa9,0xfd,0xb7};
 byte LIGHT4[] = {0x55,0x01,0x00,0x65,0x29,0xb6,0x91,0x0d,0xa4,0xda,0x69,0x51,0x12,0x49,0x36,0x93,0x8d,0x74,0xb3};
 byte LIGHT5[] = {0x55,0x01,0x00,0x44,0x2f,0xff,0xdd,0xbd,0xf7,0xff,0x7f,0x40,0x36,0xd3,0x4d,0x36,0x73,0x2d,0x96};
 byte LIGHT6[] = {0x55,0x01,0x00,0x42,0x09,0x64,0xb0,0x4b,0x2c,0x92,0x59,0x21,0x12,0x69,0x36,0x93,0x89,0xe4,0xb1};
 byte* LIGHT_CODE[] = {LIGHT1,LIGHT2,LIGHT3,LIGHT4,LIGHT5,LIGHT6};