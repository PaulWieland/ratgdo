TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT / HomeKit Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)

# Configuration
This section only applies to standard version which uses WiFi & MQTT. The No WiFi version does not have any configuration options.

After the firmware is flashed, the ratgdo will reboot and attempt to connect to your wifi. Once this is succesfull, click "Visit Device" from the ESP Web Tools page and you will be presented with ratgdo's configuration page. 

**Security Note:** after the initial configuraiton is made, the device's config page will be loosely protected by an http basic authentication challenge. The username is the device name (case sensitive) and the password is the OTA Password that you specified. For stronger protection, check the _Disable OTA_ checkbox and the firmware will completely disable the http web & ArduinoOTA services. With these services disabled, the only way to update the configuration is to reflash the firmware with a USB cable. 

**Configuration Fields:**

* **Device Name** - Must be unique. Used as the device's hostname on the network, the mqtt device name, part of the path on the MQTT Topic (see MQTT prefix below), and the username for basic authentication to the ESP's built in web server.
* **OTA Password** - Used for Over The Air firmware flashing and as the http basic authentication password for the web server configuration page. Use a very strong password if you leave the ota/web service enabled since anyone with access to ratgdo over the network could also gain control over your garage door.
* **IP Address** - Will be filled in automatically with the DHCP address. Use a DHCP reservation if you wish to have a static address.
* **SSID** - Your WiFi network name (case sensitive).
* **WiFi Password** - The password for the WiFi.
* **Enable MQTT** - Leave checked. If you do not wish to use MQTT, there is no reason to use the WiFi version of the firmware.
* **MQTT Server IP** - Required. The address of your MQTT broker.
* **MQTT Server Port** - Required. The port used for MQTT communication. 1883 is the default port.
* **MQTT Server Username & Password** - Optional. The Username & Password for authentication to the MQTT broker.
* **MQTT Topic Prefix** - Required. This is the prefix used for creating the MQTT topic that ratgdo will publish & subscribe to. `TOPIC_PREFIX/DEVICE_NAME/[command|status]`
    * Example: With Device Name of `MainDoor` and a topic prefix of `/home/garage/`, ratgdo will subscribe to mqtt topic `/home/garage/MainDoor/command` and it will publish to `/home/garage/MainDoor/status`. <em>Note</em> If you are using Home Assistant, do not put a space or any "illegal" characters in your device name or prefix, otherwise HA will not be able to add the device.
* **Home Assistant Discovery Prefix - `homeassistant` is the default prefix that Home Assistant uses. If you changed your HA auto discovery configuration, then update this setting.
* **Disable OTA & Web Server Config Access** - will disable the ArduinoOTA & Web service for additional security. If you use this option, you will have to reflash the firmware with a USB cable in order to change any config settings. 
* **Control Protocol** - Required. 
	* Chamberlain / LiftMaster with YELLOW learn button (Except some wall mounted jackshaft openers): Choose Security + 2.0
	* Chamberlain / LiftMaster with PURPLE or RED learn button (And some wall mounted jackshaft openers): Choose Security + 1.0
	* Other openers - choose Dry Contact
