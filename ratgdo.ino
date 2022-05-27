#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <BootstrapManager.h> // Must use the https://github.com/PaulWieland/arduinoImprovBootstrapper fork

/************* MQTT TOPICS **************************/
String doorCommandTopic = "";
String doorCommand = ""; // will be [open|close|light]

String doorStatusTopic = "";
String doorStatus = ""; // will be [online|offline|open|closed|opening|closing|obstructed|clear]


BootstrapManager bootstrapManager;

int lastMillis = 0;
bool test = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Begin setup");

  bootstrapManager.bootstrapSetup(manageDisconnections, manageHardwareButton, callback);

  // Setup mqtt topics
  doorCommandTopic = String(mqttTopicPrefix) + deviceName + "/command";
  doorStatusTopic = String(mqttTopicPrefix) + deviceName + "/status";

  // Broadcast that we are online
  bootstrapManager.publish(doorStatusTopic.c_str(), "online", false);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH); // led off to indicate no connection
}

void loop() {
  if (isConfigFileOk) {

    // Bootsrap loop() with Wifi, MQTT and OTA functions
    bootstrapManager.bootstrapLoop(manageDisconnections, manageQueueSubscription, manageHardwareButton);

    //    Serial.println("Hello World");
    //    Serial.println(mqttTopicPrefix);
    //    Serial.println(deviceName);
    //    Serial.println(doorCommandTopic);
    //    delay(10000);


    // put your main code here, to run repeatedly:
    if (millis() > lastMillis + 600) {
      if (test) {
        test = false;
        digitalWrite(BUILTIN_LED, LOW);
        //Serial.println("blink low");
      } else {
        // Send MSG to the MQTT queue with no retention
//        bootstrapManager.publish(doorStatusTopic.c_str(), "closed", false);

        test = true;
        digitalWrite(BUILTIN_LED, HIGH);
        //Serial.println("blink high");
      }

      lastMillis = millis();
    }

  }



}

/********************************** MANAGE WIFI AND MQTT DISCONNECTION *****************************************/
void manageDisconnections() {

}

/********************************** MQTT SUBSCRIPTIONS *****************************************/
void manageQueueSubscription() {

  // example to topic subscription
  bootstrapManager.subscribe(doorCommandTopic.c_str());
  Serial.print("Subscribed to ");
  Serial.println(doorCommandTopic);

}

/********************************** MANAGE HARDWARE BUTTON *****************************************/
void manageHardwareButton() {

}

/********************************** START CALLBACK *****************************************/
void callback(char* topic, byte* payload, unsigned int length) {

  // Transform all messages in a JSON format
  StaticJsonDocument<BUFFER_SIZE> json = bootstrapManager.parseQueueMsg(topic, payload, length);

  doorCommand = (String) json[VALUE];
  Serial.println(doorCommand);

  if (doorCommand == "open") {
    Serial.println("open the pod bay door");
  } else if (doorCommand == "close") {
    Serial.println("close sesame");
  } else if (doorCommand == "light") {
    Serial.println("let the be light (or not)");
  } else {
    Serial.println("Unknown command, ignoring");
  }
}
