#include "home_assistant.h"

String uniqueID;

void ha_autodiscovery_setup(BootstrapManager * bootstrapManager){
	uniqueID = deviceName + "_" + WiFi.macAddress();

	ha_autodiscovery_door(bootstrapManager);
	ha_autodiscovery_light(bootstrapManager);
	ha_autodiscovery_obs(bootstrapManager);
}

void ha_autodiscovery_door(BootstrapManager *bootstrapManager){
	String topic = "homeassistant/cover/"+ deviceName +"/config";
	JsonObject payload = bootstrapManager->getJsonObject();

	payload["~"] = mqttTopicPrefix + deviceName;
    payload["name"] = deviceName;
    payload["unique_id"] = uniqueID;
    payload["availability_topic"] = "~/status/availability";
    payload["device_class"] = "garage";
    payload["command_topic"] = "~/command";
    payload["payload_open"] = "open";
    payload["payload_close"] = "close";
    payload["payload_stop"] = "stop";
    payload["state_topic"] = "~/status/door";


	JsonObject device = payload.createNestedObject("device");
	device["name"] = deviceName;
    device["identifiers"] = uniqueID;
	device["manufacturer"] = "Paul Wieland";
	device["model"] = "ratgdo";
	device["sw_version"] = VERSION;
	device["configuration_url"] = "http://"+microcontrollerIP+"/";

	bootstrapManager->publish(topic.c_str(), payload, true);
}

void ha_autodiscovery_light(BootstrapManager * bootstrapManager){
	String topic = "homeassistant/button/"+ deviceName +"/config";
	JsonObject payload = bootstrapManager->getJsonObject();

	payload["~"] = mqttTopicPrefix + deviceName;
	payload["name"] = deviceName + " Light";
	payload["unique_id"] = uniqueID + "_light";
	payload["availability_topic"] = "~/status/availability";
    payload["command_topic"] = "~/command";
    payload["payload_press"] = "light";

	JsonObject device = payload.createNestedObject("device");
	device["name"] = deviceName;
    device["identifiers"] = uniqueID;
	device["manufacturer"] = "Paul Wieland";
	device["model"] = "ratgdo";
	device["sw_version"] = VERSION;
	device["configuration_url"] = "http://"+microcontrollerIP+"/";

	bootstrapManager->publish(topic.c_str(), payload, true);
}

void ha_autodiscovery_obs(BootstrapManager *bootstrapManager){
	String topic = "homeassistant/binary_sensor/"+ deviceName + "/config";
	JsonObject payload = bootstrapManager->getJsonObject();

	payload["~"] = mqttTopicPrefix + deviceName;
    payload["name"] = deviceName + " Obstruction";
    payload["unique_id"] = uniqueID + "_obs";
    payload["availability_topic"] = "~/status/availability";
    payload["device_class"] = "motion";
    payload["state_topic"] = "~/status/obstruction";
    payload["payload_on"] = "obstructed";
    payload["payload_off"] = "clear";

   	JsonObject device = payload.createNestedObject("device");
	device["name"] = deviceName;
    device["identifiers"] = uniqueID;
	device["manufacturer"] = "Paul Wieland";
	device["model"] = "ratgdo";
	device["sw_version"] = VERSION;
	device["configuration_url"] = "http://"+microcontrollerIP+"/";

	bootstrapManager->publish(topic.c_str(), payload, true);
}