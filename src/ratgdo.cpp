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

#include "common.h"
#include "ratgdo.h"

void setup(){
	pinMode(INPUT_GDO, INPUT_PULLUP);
	pinMode(OUTPUT_GDO, OUTPUT);
	// swSerial.begin(9600, SWSERIAL_8N2, INPUT_GDO, OUTPUT_GDO, true);
	swSerial.begin(9600, SWSERIAL_8N1, INPUT_GDO, OUTPUT_GDO, true);
	
	Serial.begin(115200); // must remain at 115200 for improv
	Serial.println("");

	#ifndef DISABLE_WIFI
	bootstrapManager.bootstrapSetup(manageDisconnections, manageHardwareButton, callback);
	
	// Setup mqtt topics to subscribe to
	commandTopic = String(mqttTopicPrefix) + deviceName + "/command/#";
	setCounterTopic = String(mqttTopicPrefix) + deviceName + "/set_code_counter";

	// match these topic names
	doorCommandTopic = String(mqttTopicPrefix) + deviceName + "/command/door";
	lightCommandTopic = String(mqttTopicPrefix) + deviceName + "/command/light";
	lockCommandTopic = String(mqttTopicPrefix) + deviceName + "/command/lock";
	
	// mqtt topics to publish status updates to
	availabilityStatusTopic = String(mqttTopicPrefix) + deviceName + "/status/availability";
	obstructionStatusTopic = String(mqttTopicPrefix) + deviceName + "/status/obstruction";
	doorStatusTopic = String(mqttTopicPrefix) + deviceName + "/status/door";
	lightStatusTopic = String(mqttTopicPrefix) + deviceName + "/status/light";
	lockStatusTopic = String(mqttTopicPrefix) + deviceName + "/status/lock";
	
	rollingCodeTopic = String(mqttTopicPrefix) + deviceName + "/rolling_code_count";

	bootstrapManager.setMQTTWill(availabilityStatusTopic.c_str(),"offline",1,false,true);
	
	Serial.print("doorCommandTopic: ");
	Serial.println(doorCommandTopic);
	Serial.print("lightCommandTopic: ");
	Serial.println(lightCommandTopic);
	Serial.print("lockCommandTopic: ");
	Serial.println(lockCommandTopic);
	#endif

	pinMode(TRIGGER_OPEN, INPUT_PULLUP);
	pinMode(TRIGGER_CLOSE, INPUT_PULLUP);
	pinMode(TRIGGER_LIGHT, INPUT_PULLUP);
	pinMode(STATUS_DOOR, OUTPUT);
	pinMode(STATUS_OBST, OUTPUT);
	pinMode(INPUT_OBST, INPUT);

	attachInterrupt(TRIGGER_OPEN,isrDoorOpen,CHANGE);
	attachInterrupt(TRIGGER_CLOSE,isrDoorClose,CHANGE);
	attachInterrupt(TRIGGER_LIGHT,isrLight,CHANGE);
	attachInterrupt(INPUT_OBST,isrObstruction,CHANGE);

	delay(60); // 
	Serial.println("Setup Complete");
	Serial.println(" _____ _____ _____ _____ ____  _____ ");
	Serial.println("| __  |  _  |_   _|   __|    \\|     |");
	Serial.println("|    -|     | | | |  |  |  |  |  |  |");
	Serial.println("|__|__|__|__| |_| |_____|____/|_____|");
	Serial.print("version ");
	Serial.print(VERSION);
	#ifdef DISABLE_WIFI
	Serial.print(" (WiFi disabled)");
	#endif
	Serial.println("");

	LittleFS.begin();

	readCounterFromFlash();

	Serial.println("Syncing rolling code counter after reboot...");
	sync(); // if rolling codes are being used (rolling code counter > 0), send reboot/sync to the opener on startup

	delay(500);
}


/*************************** MAIN LOOP ***************************/
void loop(){
	if (isConfigFileOk){
		// Bootsrap loop() with Wifi, MQTT and OTA functions
		bootstrapManager.bootstrapLoop(manageDisconnections, manageQueueSubscription, manageHardwareButton);

		if(!setupComplete){
			setupComplete = true;

			// Send Home Assistant autodiscovery mqtt messages
			ha_autodiscovery_setup(&bootstrapManager);

			// Broadcast that we are online
			bootstrapManager.publish(availabilityStatusTopic.c_str(), "online", true);
		}
	}

	obstructionLoop();
	gdoStateLoop();
	dryContactLoop();
	statusUpdateLoop();
}

/*************************** DETECTING THE DOOR STATE ***************************/
void gdoStateLoop(){
	if(!swSerial.available()) return;
	uint8_t serData = swSerial.read();

	static uint32_t msgStart;
	static bool reading = false;
	static uint16_t byteCount = 0;

	if(!reading){
		// shift serial byte onto msg start
		msgStart <<= 8;
		msgStart |= serData;

		// truncate to 3 bytes
		msgStart &= 0x00FFFFFF;

		// if we are at the start of a message, capture the next 16 bytes
		if(msgStart == 0x550100){
			byteCount = 3;
			rxRollingCode[0] = 0x55;
			rxRollingCode[1] = 0x01;
			rxRollingCode[2] = 0x00;

			reading = true;
			return;
		}
	}

	if(reading){
		rxRollingCode[byteCount] = serData;
		byteCount++;

		if(byteCount == 19){
			reading = false;
			msgStart = 0;
			byteCount = 0;

			readRollingCode(rxRollingCode, doorState, lightState, lockState, obstructionState);
		}
	}
}

/*************************** DRY CONTACT CONTROL OF LIGHT & DOOR ***************************/
void IRAM_ATTR isrDebounce(const char *type){
	static unsigned long lastOpenDoorTime = 0;
	static unsigned long lastCloseDoorTime = 0;
	static unsigned long lastToggleLightTime = 0;
	unsigned long currentMillis = millis();

	// Prevent ISR during the first 2 seconds after reboot
	if(currentMillis < 2000) return;

	if(strcmp(type, "openDoor") == 0){
		if(digitalRead(TRIGGER_OPEN) == LOW){
			// save the time of the falling edge
			lastOpenDoorTime = currentMillis;
		}else if(currentMillis - lastOpenDoorTime > 500 && currentMillis - lastOpenDoorTime < 10000){
			// now see if the rising edge was between 500ms and 10 seconds after the falling edge
			dryContactDoorOpen = true;
		}
	}

	if(strcmp(type, "closeDoor") == 0){
		if(digitalRead(TRIGGER_CLOSE) == LOW){
			// save the time of the falling edge
			lastCloseDoorTime = currentMillis;
		}else if(currentMillis - lastCloseDoorTime > 500 && currentMillis - lastCloseDoorTime < 10000){
			// now see if the rising edge was between 500ms and 10 seconds after the falling edge
			dryContactDoorClose = true;
		}
	}

	if(strcmp(type, "toggleLight") == 0){
		if(digitalRead(TRIGGER_LIGHT) == LOW){
			// save the time of the falling edge
			lastToggleLightTime = currentMillis;
		}else if(currentMillis - lastToggleLightTime > 500 && currentMillis - lastToggleLightTime < 10000){
			// now see if the rising edge was between 500ms and 10 seconds after the falling edge
			dryContactToggleLight = true;
		}
	}
}

void IRAM_ATTR isrDoorOpen(){
	isrDebounce("openDoor");
}

void IRAM_ATTR isrDoorClose(){
	isrDebounce("closeDoor");
}

void IRAM_ATTR isrLight(){
	isrDebounce("toggleLight");
}

// handle changes to the dry contact state
void dryContactLoop(){
	if(dryContactDoorOpen){
		Serial.println("Dry Contact: open the door");
		dryContactDoorOpen = false;
		openDoor();
	}

	if(dryContactDoorClose){
		Serial.println("Dry Contact: close the door");
		dryContactDoorClose = false;
		closeDoor();
	}

	if(dryContactToggleLight){
		Serial.println("Dry Contact: toggle the light");
		dryContactToggleLight = false;
		toggleLight();
	}
}

/*************************** OBSTRUCTION DETECTION ***************************/
void IRAM_ATTR isrObstruction(){
	if(digitalRead(INPUT_OBST)){
		lastObstructionHigh = millis();
	}else{
		obstructionLowCount++;
	}
	
}

void obstructionLoop(){
	long currentMillis = millis();
	static unsigned long lastMillis = 0;

	// the obstruction sensor has 3 states: clear (HIGH with LOW pulse every 7ms), obstructed (HIGH), asleep (LOW)
	// the transitions between awake and asleep are tricky because the voltage drops slowly when falling asleep
	// and is high without pulses when waking up

	// If 3 low pulses are counted within 25ms, the door is awake, not obstructed and we don't have to check anything else

	// Every 25ms
	if(currentMillis - lastMillis > 25){
		// check to see if we got between 3 and 5 low pulses on the line
		if(obstructionLowCount >= 3 && obstructionLowCount <= 5){
			// obstructionCleared();
			obstructionState = 1;

		// if there have been no pulses the line is steady high or low			
		}else if(obstructionLowCount == 0){
			// if the line is high and the last high pulse was more than 50ms ago, then there is an obstruction present
			if(digitalRead(INPUT_OBST) && currentMillis - lastObstructionHigh > 50){
				obstructionState = 0;
				// obstructionDetected();
			}else{
				// asleep
			}
		}

		lastMillis = currentMillis;
		obstructionLowCount = 0;
	}
}

/*************************** STATUS UPDATES ***************************/
void statusUpdateLoop(){
	// initialize to unknown
	static uint8_t previousDoorState = 0;
	static uint8_t previousLightState = 2;
	static uint8_t previousLockState = 2;
	static uint8_t previousObstructionState = 2;

	if(doorState != previousDoorState) sendDoorStatus();
	if(lightState != previousLightState) sendLightStatus();
	if(lockState != previousLockState) sendLockStatus();
	if(obstructionState != previousObstructionState) sendObstructionStatus();

	previousDoorState = doorState;
	previousLightState = lightState;
	previousLockState = lockState;
	previousObstructionState = obstructionState;
}

void sendDoorStatus(){
	Serial.print("Door state ");
	Serial.println(doorStates[doorState]);

	if(doorState == 1) digitalWrite(STATUS_DOOR, HIGH); // Open
	if(doorState == 2) digitalWrite(STATUS_DOOR, LOW); // Closed
	
	if(isConfigFileOk){
		bootstrapManager.publish(doorStatusTopic.c_str(), doorStates[doorState].c_str(), true);
	}
}

void sendLightStatus(){
	Serial.print("Light state ");
	Serial.println(lightStates[lightState]);

	if(isConfigFileOk){
		bootstrapManager.publish(lightStatusTopic.c_str(), lightStates[lightState].c_str(), true);
	}
}

void sendLockStatus(){
	Serial.print("Lock state ");
	Serial.println(lockStates[lockState]);

	if(isConfigFileOk){
		bootstrapManager.publish(lockStatusTopic.c_str(), lockStates[lockState].c_str(), true);
	}
}

void sendObstructionStatus(){
	Serial.print("Obstruction status ");
	Serial.println(obstructionStates[obstructionState]);

	if(obstructionState == 0) digitalWrite(STATUS_OBST,HIGH); // obstructed
	if(obstructionState == 1) digitalWrite(STATUS_OBST,LOW); // clear

	if(isConfigFileOk){
		bootstrapManager.publish(obstructionStatusTopic.c_str(), obstructionStates[obstructionState].c_str(), true);
	}
}

void sendCurrentCounter(){
	String msg = String(rollingCodeCounter);
	Serial.print("Current counter ");
	Serial.println(rollingCodeCounter);
	if(isConfigFileOk){
		bootstrapManager.publish(rollingCodeTopic.c_str(), msg.c_str(), true);
	}
}

/********************************** MANAGE WIFI AND MQTT DISCONNECTION *****************************************/
void manageDisconnections(){
	setupComplete = false;
}

/********************************** MQTT SUBSCRIPTIONS *****************************************/
void manageQueueSubscription(){
	bootstrapManager.subscribe(commandTopic.c_str());
	bootstrapManager.subscribe(setCounterTopic.c_str());
}

/********************************** MANAGE HARDWARE BUTTON *****************************************/
void manageHardwareButton(){
}

/********************************** MQTT CALLBACK *****************************************/
void callback(char *topic, byte *payload, unsigned int length){

	// Transform all messages in a JSON format
	StaticJsonDocument<BUFFER_SIZE> json = bootstrapManager.parseQueueMsg(topic, payload, length);

	if(strcmp(topic,setCounterTopic.c_str()) == 0){
		String s = String((char*)payload);
		rollingCodeCounter = s.toInt();

		Serial.print("MQTT Set rolling code counter ");
		Serial.println(rollingCodeCounter);
		writeCounterToFlash();
	}

	String command = (String)json[VALUE];
	if(command == "query"){
		Serial.println("MQTT: query");

		getRollingCode("reboot2");
		transmit(txRollingCode,CODE_LENGTH);
		delay(100);

		// Set all to unknown
		doorState = 0;
		lightState = 2;
		lockState = 2;
		obstructionState = 2;
	}

	if(command == "sync"){
		Serial.println("MQTT: sync");
		sync();
	}

	if(strcmp(topic,doorCommandTopic.c_str()) == 0){
		if (command == "open"){
			Serial.println("MQTT: open the door");
			openDoor();
		}else if (command == "close"){
			Serial.println("MQTT: close the door");
			closeDoor();
		}else if (command == "stop"){
			Serial.println("MQTT: stop the door");
			stopDoor();
		}else{
			Serial.print("Unknown command: ");
			Serial.println(command);
		}
	}

	if(strcmp(topic,lightCommandTopic.c_str()) == 0){
		if (command == "on"){
			Serial.println("MQTT: turn the light on");
			lightOn();
		}else if(command = "off"){
			Serial.println("MQTT: turn the light off");
			lightOff();
		}else{
			Serial.print("Unknown command: ");
			Serial.println(command);
		}
	}

	if(strcmp(topic,lockCommandTopic.c_str()) == 0){
		if (command == "lock"){
			Serial.println("MQTT: lock");
			lock();
		}else if(command == "unlock"){
			Serial.println("MQTT: unlock");
			unlock();
		}else{
			Serial.print("Unknown command: ");
			Serial.println(command);
		}
	}
}

/************************* DOOR COMMUNICATION *************************/
/*
 * Transmit a message to the door opener over uart1
 * The TX1 pin is controlling a transistor, so the logic is inverted
 * A HIGH state on TX1 will pull the 12v line LOW
 * 
 * The opener requires a specific duration low/high pulse before it will accept a message
 */
void transmit(byte* payload, unsigned int length){
	digitalWrite(OUTPUT_GDO, HIGH); // pull the line high for 1305 micros so the door opener responds to the message
	delayMicroseconds(1305);
	digitalWrite(OUTPUT_GDO, LOW); // bring the line low

	delayMicroseconds(1260); // "LOW" pulse duration before the message start
	swSerial.write(payload, length);
}

void sync(){
	getRollingCode("reboot1");
	transmit(txRollingCode,CODE_LENGTH);
	delay(65);

	getRollingCode("reboot2");
	transmit(txRollingCode,CODE_LENGTH);
	delay(65);

	getRollingCode("reboot3");
	transmit(txRollingCode,CODE_LENGTH);
	delay(65);

	getRollingCode("reboot4");
	transmit(txRollingCode,CODE_LENGTH);
	delay(65);

	getRollingCode("reboot5");
	transmit(txRollingCode,CODE_LENGTH);
	delay(65);

	getRollingCode("reboot6");
	transmit(txRollingCode,CODE_LENGTH);
	delay(65);

	writeCounterToFlash();
}

// Door functions
void openDoor(){
	if(doorStates[doorState] == "open" || doorStates[doorState] == "opening"){
		Serial.print("The door is already ");
		Serial.println(doorStates[doorState]);
		return;
	}

	toggleDoor();
}

void closeDoor(){
	if(doorStates[doorState] == "closed" || doorStates[doorState] == "closing"){
		Serial.print("The door is already ");
		Serial.println(doorStates[doorState]);
		return;
	}

	toggleDoor();
}

void stopDoor(){
	if(doorStates[doorState] == "opening" || doorStates[doorState] == "closing"){
		toggleDoor();
	}else{
		Serial.print("The door is not moving.");
	}
}

void toggleDoor(){
	getRollingCode("door1");
	transmit(txRollingCode, CODE_LENGTH);

	delay(40);

	getRollingCode("door2");
	transmit(txRollingCode, CODE_LENGTH);

	writeCounterToFlash();
}

// Light functions
void lightOn(){
	if(lightStates[lightState] == "on"){
		Serial.println("already on");
	}else{
		toggleLight();
	}
}

void lightOff(){
	if(lightStates[lightState] == "off"){
		Serial.println("already off");
	}else{
		toggleLight();
	}
}

void toggleLight(){
	getRollingCode("light");
	transmit(txRollingCode,CODE_LENGTH);
	writeCounterToFlash();
}

// Lock functions
void lock(){
	if(lockStates[lockState] == "locked"){
		Serial.println("already locked");
	}else{
		toggleLock();
	}
}

void unlock(){
	if(lockStates[lockState] == "unlocked"){
		Serial.println("already unlocked");
	}else{
		toggleLock();
	}
}

void toggleLock(){
	getRollingCode("lock");
	transmit(txRollingCode,CODE_LENGTH);
	writeCounterToFlash();
}