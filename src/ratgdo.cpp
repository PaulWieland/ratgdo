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
    Serial1.begin(9600, SERIAL_8N2, SERIAL_FULL, OUTPUT_GDO, true); // 9600 baud/invert signal for door opener com
    Serial.begin(115200);
    Serial.println("Begin setup");

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
    pinMode(INPUT_RPM1, INPUT);
    pinMode(INPUT_RPM2, INPUT);
    pinMode(INPUT_OBST, INPUT);

    attachInterrupt(TRIGGER_OPEN,isrDoorOpen,RISING);
    attachInterrupt(TRIGGER_CLOSE,isrDoorClose,RISING);
    attachInterrupt(TRIGGER_LIGHT,isrLight,RISING);
    attachInterrupt(INPUT_OBST,isrObstruction,FALLING);

    delay(60); // 
    Serial.println("Setup Complete");
    Serial.println(" _____ _____ _____ _____ ____  _____ ");
    Serial.println("| __  |  _  |_   _|   __|    \\|     |");
    Serial.println("|    -|     | | | |  |  |  |  |  |  |");
    Serial.println("|__|__|__|__| |_| |_____|____/|_____|");
    Serial.println("");
    Serial.print("v");
    Serial.println(VERSION);
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
    static int counter = 0;
    static int lastCounter = 0;
    static int lastCounterMillis = 0;
    static int RPM2_lastState = 1;
    int RPM2_state = digitalRead(INPUT_RPM2);

    // If the previous and the current state of the RPM2 Signal are different, that means the door is moving
    if(RPM2_state != RPM2_lastState){
        lastCounterMillis = millis();

        // If the RPM2 state is different to the RPM1 state, that means the door is opening
        // If the two are equal, the door is closing
        if (digitalRead(INPUT_RPM1) != RPM2_state){
            counter--; // Door is opening (sprocket spins clockwise when viewed from below)
        }else{
            counter++; // Door is closing (sprocket spins counter clockwise when viewed from below)
        }
        Serial.print("Position: ");
        Serial.println(counter);
    }

    // Wait 5 pulses before updating to door opening status
    if(counter - lastCounter > 5){
        if(doorState != "opening"){
            Serial.println("door opening...");
            if(isConfigFileOk){
                bootstrapManager.publish(doorStatusTopic.c_str(), "opening", false);
            }
        }
        lastCounter = counter;
        doorState = "opening";
    }

    if(lastCounter - counter > 5){
        if(doorState != "closing"){
            Serial.println("door closing...");
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
            Serial.println("CLOSED");
            if(isConfigFileOk){
                bootstrapManager.publish(doorStatusTopic.c_str(), doorState.c_str(), false);
            }
            digitalWrite(STATUS_DOOR,LOW);
        }

        // if the door was opening, and is now stopped, then the door is open
        if(doorState == "opening"){
            doorState = "open";
            Serial.println("OPEN");
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
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    // Prevent ISR during the first x seconds after reboot
    if(interrupt_time < 2000){
        return;
    }

    // Anything less than 200ms is a bounce and is ignored
    if(interrupt_time - last_interrupt_time > 1000){
        if(strcmp(type, "openDoor") == 0){
            dryContactDoorOpen = true;
        }else if(strcmp(type, "closeDoor") == 0){
            dryContactDoorClose = true;
        }else if(strcmp(type, "toggleLight") ==0){
            dryContactToggleLight = true;
        }
    }
    last_interrupt_time = interrupt_time;
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

// handle changes 
void dryContactLoop(){
    if(dryContactDoorOpen){
        Serial.println("dry contact: open the door");
        dryContactDoorOpen = false;
        openDoor();
    }
    if(dryContactDoorClose){
        Serial.println("dry contact: close the door");
        dryContactDoorClose = false;
        closeDoor();
    }

    if(dryContactToggleLight){
        Serial.println("dry contact: toggle the light");
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

        Serial.println("obstruction detected");

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

        Serial.println("obstruction cleared");

        if(isConfigFileOk){
            bootstrapManager.publish(doorStatusTopic.c_str(), "clear", false);
        }
    }
}

/********************************** MANAGE WIFI AND MQTT DISCONNECTION *****************************************/
void manageDisconnections(){
    setupComplete = false;

    // If WiFi is configured, but no connection Blink the LED
    // if(isConfigFileOk){
    //     static bool ledState = false;
    //     static unsigned long lastMillis = 0;
        
    //     if(millis() - lastMillis > 500){
    //         if(ledState){
    //             ledState = false;
    //             digitalWrite(LED_BUILTIN, HIGH); // led off
    //         }else{
    //             ledState = true;
    //             digitalWrite(LED_BUILTIN, HIGH); // led on
    //         }
    //         lastMillis = millis();
    //     }
    // }
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
        Serial.println("mqtt open the door");
        openDoor();
    }else if (doorCommand == "close"){
        Serial.println("mqtt close the door");
        closeDoor();
    }else if (doorCommand == "light"){
        Serial.println("mqtt toggle the light");
        toggleLight();
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
 * Swap the TX1 pin for GPIO, manually send pulse, then swap back to serial1
 */
void transmit(const byte* payload, unsigned int length){
  /********** CHANGE TX PIN FUNCTION  TO GPIO **********/
  Serial1.end(); // End Serial1 session so we can manually bring the transmit line high for a specific duration
  pinMode(OUTPUT_GDO, OUTPUT); // GDO/D4/Serial1 (TX) swap the pin to a GPIO.
  /*****************************************************/

  digitalWrite(OUTPUT_GDO, HIGH); // pull the line high for 1305 micros so the door opener responds to the message
  delayMicroseconds(1305);
  digitalWrite(OUTPUT_GDO, LOW); // bring the line low


  /********** CHANGE PIN FUNCTION  TO TX/RX **********/
  pinMode(OUTPUT_GDO, FUNCTION_0); // GDO/D4 Serial1 (TX) swap the pin to uart TX.
  Serial1.begin(9600, SERIAL_8N2, SERIAL_FULL, OUTPUT_GDO, true);
  /***************************************************/

  delayMicroseconds(960); // "LOW" pulse duration before the message start

  Serial1.write(payload, length); // send the code to the door opener  
}

void openDoor(){
    if(doorState == "open") return;
    for(int i=0; i<7; i++){
        Serial.print("door_code[");
        Serial.print(i);
        Serial.println("]");

        transmit(DOOR_CODE[i],19);
        delay(45);
    }
}

void closeDoor(){
    if(doorState == "closed") return;
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