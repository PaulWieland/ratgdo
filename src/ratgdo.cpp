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

#include "ratgdo.h"

void setup(){
    swSerial.begin(9600, SWSERIAL_8N2, -1, OUTPUT_GDO, true);
    
    Serial.begin(115200);
    Serial.println("");

    #ifndef DISABLE_WIFI
    bootstrapManager.bootstrapSetup(manageDisconnections, manageHardwareButton, callback);
    
    // Setup mqtt topics
    doorCommandTopic = String(mqttTopicPrefix) + deviceName + "/command";
    doorStatusTopic = String(mqttTopicPrefix) + deviceName + "/status";

    bootstrapManager.setMQTTWill(doorStatusTopic.c_str(),"offline",1,false,true);
    
    Serial.print("doorCommandTopic: ");
    Serial.println(doorCommandTopic);
    Serial.print("doorStatusTopic: ");
    Serial.println(doorStatusTopic);
    #endif

    // pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, HIGH); // led off

    pinMode(TRIGGER_OPEN, INPUT_PULLUP);
    pinMode(TRIGGER_CLOSE, INPUT_PULLUP);
    pinMode(TRIGGER_LIGHT, INPUT_PULLUP);
    pinMode(STATUS_DOOR, OUTPUT);
    pinMode(STATUS_OBST, OUTPUT);
    pinMode(INPUT_RPM1, INPUT_PULLUP); // set to pullup to add support for reed switches
    pinMode(INPUT_RPM2, INPUT);
    pinMode(INPUT_OBST, INPUT);

    attachInterrupt(TRIGGER_OPEN,isrDoorOpen,CHANGE);
    attachInterrupt(TRIGGER_CLOSE,isrDoorClose,CHANGE);
    attachInterrupt(TRIGGER_LIGHT,isrLight,CHANGE);
    attachInterrupt(INPUT_OBST,isrObstruction,FALLING);

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

}


/*************************** MAIN LOOP ***************************/
void loop(){
    if (isConfigFileOk){
        // Bootsrap loop() with Wifi, MQTT and OTA functions
        bootstrapManager.bootstrapLoop(manageDisconnections, manageQueueSubscription, manageHardwareButton);

        if(!setupComplete){
            setupComplete = true;
            
            // Broadcast that we are online
            bootstrapManager.publish(doorStatusTopic.c_str(), "online", false);
        }
    }

    obstructionLoop();
    doorStateLoop();
    dryContactLoop();
}

/*************************** DETECTING THE DOOR STATE ***************************/
void doorStateLoop(){
    static bool rotaryEncoderDetected = false;
    static int counter = 0;
    static int lastCounter = 0;
    static int lastCounterMillis = 0;
    static int RPM2_lastState = 1;
    int RPM2_state = digitalRead(INPUT_RPM2);

    // Handle reed switch
    // This may need to be debounced, but so far in testing I haven't detected any bounces
    if(!rotaryEncoderDetected){
        if(digitalRead(INPUT_RPM1) == LOW){
            if(doorState != "reed_closed"){
                Serial.println("Reed switch closed");
                doorState = "reed_closed";
                if(isConfigFileOk){
                    bootstrapManager.publish(doorStatusTopic.c_str(), "reed_closed", false);
                }
                digitalWrite(STATUS_DOOR,HIGH);
            }
        }else if(doorState != "reed_open"){
            Serial.println("Reed switch open");
            doorState = "reed_open";
            if(isConfigFileOk){
                bootstrapManager.publish(doorStatusTopic.c_str(), "reed_open", false);
            }
            digitalWrite(STATUS_DOOR,LOW);
        }
    }
    // end reed switch handling

    // If the previous and the current state of the RPM2 Signal are different, that means there is a rotary encoder detected and the door is moving
    if(RPM2_state != RPM2_lastState){
        rotaryEncoderDetected = true; // this disables the reed switch handler
        lastCounterMillis = millis();

        // If the RPM2 state is different to the RPM1 state, that means the door is opening
        // If the two are equal, the door is closing
        if (digitalRead(INPUT_RPM1) != RPM2_state){
            counter--; // Door is closing (sprocket spins clockwise when viewed from below)
        }else{
            counter++; // Door is opening (sprocket spins counter clockwise when viewed from below)
        }
        Serial.print("Door Position: ");
        Serial.println(counter);
    }

    // Wait 5 pulses before updating to door opening status
    if(counter - lastCounter > 5){
        if(doorState != "opening"){
            Serial.println("Door Opening...");
            if(isConfigFileOk){
                bootstrapManager.publish(doorStatusTopic.c_str(), "opening", false);
            }
        }
        lastCounter = counter;
        doorState = "opening";
    }

    if(lastCounter - counter > 5){
        if(doorState != "closing"){
            Serial.println("Door Closing...");
            if(isConfigFileOk){
                bootstrapManager.publish(doorStatusTopic.c_str(), "closing", false);
            }
        }
        lastCounter = counter;
        doorState = "closing";
    }

    // 250 millis after the last rotary encoder pulse, the door is stopped
    if(millis() - lastCounterMillis > 250){
        // if the door was closing, and is now stopped, then the door is closed
        if(doorState == "closing"){
            doorState = "closed";
            Serial.println("Closed");
            if(isConfigFileOk){
                bootstrapManager.publish(doorStatusTopic.c_str(), doorState.c_str(), false);
            }
            digitalWrite(STATUS_DOOR,LOW);
        }

        // if the door was opening, and is now stopped, then the door is open
        if(doorState == "opening"){
            doorState = "open";
            Serial.println("Open");
            if(isConfigFileOk){
                bootstrapManager.publish(doorStatusTopic.c_str(), doorState.c_str(), false);
            }
            digitalWrite(STATUS_DOOR,HIGH);
        }
    }

    RPM2_lastState = RPM2_state;
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
    obstructionTimer = millis();
}

void obstructionLoop(){
    long currentMillis = millis();

    // as long as a low pulse was detected within 8 millis, there is no obstruction
    if(currentMillis - obstructionTimer > 15){
        obstructionDetected();
    }else{
        obstructionCleared();
    }
}

void obstructionDetected(){
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // Anything less than 100ms is a bounce and is ignored
    if(interrupt_time - last_interrupt_time > 250){
        doorIsObstructed = true;
        digitalWrite(STATUS_OBST,HIGH);

        Serial.println("Obstruction Detected");

        if(isConfigFileOk){
            bootstrapManager.publish(doorStatusTopic.c_str(), "obstructed", false);
        }
    }
    last_interrupt_time = interrupt_time;
}

void obstructionCleared(){
    if(doorIsObstructed){
        doorIsObstructed = false;
        digitalWrite(STATUS_OBST,LOW);

        Serial.println("Obstruction Cleared");

        if(isConfigFileOk){
            bootstrapManager.publish(doorStatusTopic.c_str(), "clear", false);
        }
    }
}

void sendDoorStatus(){
    if(isConfigFileOk){
        Serial.print("Door state ");
        Serial.println(doorState);
        bootstrapManager.publish(doorStatusTopic.c_str(), doorState.c_str(), false);
    }
}

/********************************** MANAGE WIFI AND MQTT DISCONNECTION *****************************************/
void manageDisconnections(){
    setupComplete = false;
}

/********************************** MQTT SUBSCRIPTIONS *****************************************/
void manageQueueSubscription(){
    // example to topic subscription
    bootstrapManager.subscribe(doorCommandTopic.c_str());
    Serial.print("Subscribed to ");
    Serial.println(doorCommandTopic);
}

/********************************** MANAGE HARDWARE BUTTON *****************************************/
void manageHardwareButton(){
}

/********************************** MQTT CALLBACK *****************************************/
void callback(char *topic, byte *payload, unsigned int length){

    // Transform all messages in a JSON format
    StaticJsonDocument<BUFFER_SIZE> json = bootstrapManager.parseQueueMsg(topic, payload, length);

    doorCommand = (String)json[VALUE];
    Serial.println(doorCommand);

    if (doorCommand == "open"){
        Serial.println("MQTT: open the door");
        openDoor();
    }else if (doorCommand == "close"){
        Serial.println("MQTT: close the door");
        closeDoor();
    }else if (doorCommand == "light"){
        Serial.println("MQTT: toggle the light");
        toggleLight();
    }else if(doorCommand == "query"){
        Serial.println("MQTT: query");
        sendDoorStatus();
    }else{
        Serial.println("Unknown mqtt command, ignoring");
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
void transmit(const byte* payload, unsigned int length){
  digitalWrite(OUTPUT_GDO, HIGH); // pull the line high for 1305 micros so the door opener responds to the message
  delayMicroseconds(1305);
  digitalWrite(OUTPUT_GDO, LOW); // bring the line low

  delayMicroseconds(960); // "LOW" pulse duration before the message start
  swSerial.write(payload, length);
}

void openDoor(){
    if(doorState == "open" || doorState == "opening"){
        Serial.print("The door is already ");
        Serial.println(doorState);
        return;
    }

    doorState = "opening"; // It takes a couple of pulses to detect opening/closing. by setting here, we can avoid bouncing from rapidly repeated commands

    for(int i=0; i<7; i++){
        Serial.print("door_code[");
        Serial.print(i);
        Serial.println("]");

        transmit(DOOR_CODE[i],19);
        delay(45);
    }
}

void closeDoor(){
    if(doorState == "closed" || doorState == "closing"){
        Serial.print("The door is already ");
        Serial.println(doorState);
        return;
    }

    doorState = "closing"; // It takes a couple of pulses to detect opening/closing. by setting here, we can avoid bouncing from rapidly repeated commands

    for(int i=0; i<7; i++){
        Serial.print("door_code[");
        Serial.print(i);
        Serial.println("]");

        transmit(DOOR_CODE[i],19);
        delay(45);
    }
}

void toggleLight(){
    for(int i=0; i<6; i++){
        Serial.print("light_code[");
        Serial.print(i);
        Serial.println("]");

        transmit(LIGHT_CODE[i],19);
        delay(45);
    }
}