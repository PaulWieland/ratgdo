# ratgdo 
ratgdo gives you **local** MQTT & dry contact control plus status feedback for your Chamberlain/LiftMaster Security+ 2.0 garage door opener. Security+ 2.0 uses an encrypted serial signal to control the door opener's open/close and light functions, which makes it impossible to use Shelly One, Go Control, Insteon I/O linc or any other dry contact relay device to control the door. 

ratgdo is a hardware shield (circuit board) & firmware for the ESP8266 based Wemos D1 Mini development board that wires to your door opener's terminals and restores dry contact control. It also allows you to control the door with MQTT over your local WiFi network which can be used to integrate direclty with NodeRED or Home Assistant, eliminating the need for another "smart" device.

ratgdo is *not* a cloud device and does *not* require a subscription service. The firmware is open source and free for anyone to use.

> **ratgdo shields are available for pre order**
> The first batch of shields are expected to be available by mid August 2022
>
> * [ratgdo shield only](https://square.link/u/xNP2Orez) $15
> * [ratgdo shield with ESP8266 D1 Clone](https://square.link/u/JaMwtjLL) $30

## Features
The door opener has a rotary encoder built into it which allows the opener to know the exact position of the door. By soldering two wires to the logic board, ratgdo can monitor this rotary encoder so that it knows the exact state of the door.

For those that do not want to solder, there is an option to use an external reed switch which will give a less detailed binary status of either reed_open or reed_closed. See *Connecting* section below.

### MQTT
#### Triggers
The following MQTT commands are supported:

* open<sup>1</sup> - opens the door.
* close<sup>1</sup> - closes the door.
* light<sup>3</sup> - toggles the light on or off.

#### Statuses
The following statuses are broadcast over MQTT:

* online - once ratgdo connects to the MQTT broker.
* offline - the mqtt last will message which is broadcast by the broker when the ratgdo client loses its connection.
* obstructed - when an object breaks the obstruction sensor beam.
* clear - when an obstruction is cleared.
* opening<sup>1</sup> - when the door is opening.
* open<sup>1</sup> - when the door is fully open.
* closing<sup>1</sup> - when the door is closing.
* closed<sup>1</sup> - when the door is fully closed.
* reed_closed<sup>5</sup> - the reed switch is open (magnet is near the switch).
* reed_open<sup>5</sup> - the reed switch is closed (magnet is not near the switch).


### Dry contacts

#### Triggers
The following dry contact triggers on the ratgdo shield are at 3.3v<sup>4</sup> and can be pulled to ground to trigger the door opener as follows:

* open<sup>2</sup> - opens the door.
* close<sup>2</sup> - closes the door.
* light<sup>3</sup> - toggles the light on or off.

If you do not want to use the discrete open/close dry contacts and wish to have a toggle instead, then simply connect your switch/relay to both the open & close terminals on the ratgdo shield.

#### Statuses
The following dry contact statuses are available:

* door - 
  * When using a reed switch: 3.3v if the reed switch is closed, 0v if the reed switch is open.
  * When using the door's rotary encoder: 3.3v if the door is open, 0v if the door is closed.
* obs - 3.3v if an obstruction is detected. 0v if no obstruction.


### Notes
1. <sup>1</sup> Only when using the door opener's rotary encoder, which requires soldering to the logic board. See _Soldered Method_ below.
1. <sup>2</sup> Repeated open commands (or repeated close commands) will be ignored when using the rotary encoder. This gives discrete open/close control over the door which is better than a toggle. This is only reliable when ratgdo knows the doors position, which is only possible when connecting it to the door opener's rotary encoder.
1. <sup>3</sup> Unfortunately the door opener does not support discrete on/off commands for controlling the light, so the best we can do is toggle it.
1. <sup>4</sup> Using the ESP8266's internal INPUT_PULLUP resistor.
1. <sup>5</sup> Not used when using the door opener's rotary encoder.

# ratgdo esp flasher
These instructions explain how to flash the ratgdo firmware onto your ESP8266 directly from your browser (Google Chrome or Chromium based browsers required).

# Instructions

## Installing the firmware
1. You must use Google Chrome or a Chromium based browser. No other browsers support serial device communication at this time.
2. Connect ESP8266 with USB cable. It's recommended to do this with the ESP8266 removed from the ratgdo shield.
3. [Click here to launch the ratgdo ESP Web Tools flasher](flash.html)
4. Choose the version you wish to install
  * **ratgdo** - includes WiFi configuration through ESP Tools. After flashing the firmware, ESP tools will ask you for your WiFi credentials. After the ESP8266 connects to your network, you will be given the option to _Vist Device_ so you can configure the MQTT settings.
  * **ratgdo (WiFi Disabled)** - If you do not wish to use WiFi or MQTT, choose the _WiFi Disabled_ version. With this version you can control the garage door with the ratgdo dry contacts only.

## Configuration
This section only applies to standard version which uses WiFi & MQTT. The No WiFi version does not have any configuration options.

After the firmware is flashed, the ESP8266 will reboot and attempt to connect to your wifi. Once this is succesfull, click "Visit Device" from the ESP Web Tools page and you will be presented with ratgdo's configuration page. 

**Security Note:** after the initial configuraiton is made, the device's config page will be loosely protected by an http basic authentication challenge. The username is the device name (case sensitive) and the password is the OTA Password that you specified. For stronger protection, leave the OTA Password blank and the firmware will completely disable the web service that loads the configuration page.

**Configuration Fields:**

* **Device Name** - Must be unique. Used as the device's hostname on the network, the mqtt device name, part of the path on the MQTT Topic (see MQTT prefix below), and the username for basic authentication when editing the page.
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


## Connecting the ratgdo shield to your Security+ 2.0® garage door opener

### Soldered method - uses door opener's rotary encoder and power supply
The soldered method requires you to solder three wires to the door opener's logic board. This method provides a couple of benefits.

1. First, it allows ratgdo to know whether the garage door is opening, open, closing or closed. These statuses are broadcast out to the MQTT status topic.
2. Second, no additional hardware such as a USB Charger, usb cable, or reed switch are needed.
3. Last, the installation is cleaner looking.

Soldering to the logic board is quite easy. These instructions are for a LiftMaster Model 8365W.

Things you will need:

* A ratgdo shield **[Buy shield only](https://square.link/u/xNP2Orez)**
* A wemos D1 Mini pin compatible ESP8266 development board **[Buy shield + D1 mini Clone](https://square.link/u/JaMwtjLL)**
* A soldering iron & basic soldering skills. Good solder (lead/tin works best) & flux.00
* 3 pieces of thin gauge flexible wire, approximately 1' long (length depends on where you want to physically mount the ratgdo shield)
* A phillips screw driver or 1/4" nut driver
* A small flat blade screw driver for the ratgdo terminals
* Two small zip ties, one for strain relief of the wires and the other to mount the board

Proceedure:

1. IMPORTANT: Unplug the door opener to physically disconnect power. **Failure to do so can kill you** as there is live main voltage present on the logic board.
2. Open the white plastic rear light cover where the terminals are for the obstruction sensor & hard wired remote.
3. Disconnect the wires from the obstruction sensor and hard wired button from the push terminals.
4. Remove the three 1/4"/Phillips drive screws that hold the black rear cover to the metal chassis.
5. The logic board is attached to this cover and will come out with the cover. There are two main cables connecting the logic board to the motor and rotary encoder. 
    * Optional - disconnect the two cable connectors if you want to completely remove the back cover and logic board for soldering.
6. Locate the RPM1, RPM2 and 3.3v pins on the logic board. See diagram.
7. Solder a wire to each of these pins. Make sure you keep track of which wire is which. Hooking them up to the ratgdo shield incorrectly will cause a malfunction.
8. Route the wires as shown in the picture. Use the small zip tie to provide strain relief.
9. Reconnect the logic board cables if you disconnected them in step 5, and reinstall the cover.
10. Connect the 6 wires from the door opener to the ratgdo shield according to this diagram:

### No Solder method - external power supply and reed switch
With this method no soldering is required, however the door states of opening, open, closing, closed are unknown. Instead of providing specific door states, the ratgdo firmware simply broadcasts whether the reed switch is open or closed. The door status output will be set to HIGH when the reed switch is closed and LOW otherwise.

Things you will need:

* A ratgdo shield **[Buy shield only](https://square.link/u/xNP2Orez)**
* A wemos D1 Mini pin compatible ESP8266 development board **[Buy shield + D1 mini Clone](https://square.link/u/JaMwtjLL)**
* A Micro USB cable
* A USB power brick
* Reed switch
* A small flat blade screw driver for the ratgdo terminals
* A small zip tie for mounting the board

Proceedure:

1. Mount a reed switch to the door track, and afix the magnet to the door in such a way that the reed switch will be activated when the door is fully open (or fully closed) and deactivated once the door moves.
2. Power the ESP8266 with the USB power brick & cable
3. Connect the reed switch (two wires) plus the door opener (three wires) to the ratgdo shield according to this diagram:


### Mounting Suggestion
The ratgdo sheld has a pair of holes in it which can be used to physically mount the board. The easiest method is to use a zip tie to mount the board to the slots in the translucent white light cover.



## Disclaimer
Use this device at your own risk.