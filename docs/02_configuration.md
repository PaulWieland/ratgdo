TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [FAQ & Troubleshooting](09_faq.md)

# Configuration
This section only applies to standard version which uses WiFi & MQTT. The No WiFi version does not have any configuration options.

After the firmware is flashed, the ESP8266 will reboot and attempt to connect to your wifi. Once this is succesfull, click "Visit Device" from the ESP Web Tools page and you will be presented with ratgdo's configuration page. 

**Security Note:** after the initial configuraiton is made, the device's config page will be loosely protected by an http basic authentication challenge. The username is the device name (case sensitive) and the password is the OTA Password that you specified. For stronger protection, leave the OTA Password blank and the firmware will completely disable the web service that loads the configuration page.

**Configuration Fields:**

* **Device Name** - Must be unique. Used as the device's hostname on the network, the mqtt device name, part of the path on the MQTT Topic (see MQTT prefix below), and the username for basic authentication to the ESP's built in web server.
* **IP Address** - Will be filled in automatically with the DHCP address, but can also be set manually.
* **SSID** - Your WiFi network name (case sensitive).
* **WiFi Password** - The password for the WiFi.
* **OTA Password** - Used for Over The Air firmware flashing. If left blank, OTA and the configuration webservice will be disabled. Use a very strong password if you provide one as anyone with access to the ESP8266 over the network could also gain control over your garage door.
* **Enable MQTT** - Leave checked. If you do not wish to use MQTT, there is no reason to use the WiFi version of the firmware.
* **MQTT Server IP** - Required. The address of your MQTT broker.
* **MQTT Server Port** - Required. The port used for MQTT communication. 1883 is the default port.
* **MQTT Server Username & Password** - Optional. The Username & Password for authentication to the MQTT broker.
* **MQTT Topic Prefix** - Required. This is the prefix used for creating the MQTT topic that ratgdo will publish & subscribe to. `TOPIC_PREFIX/DEVICE_NAME/[command|status]`
    * Example: With Device Name of `Main Door` and a topic prefix of `/home/garage/`, ratgdo will subscribe to mqtt topic `/home/garage/Main Door/command` and it will publish to `/home/garage/Main Door/status`.