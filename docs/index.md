TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [FAQ & Troubleshooting](09_faq.md)


# Quick Start

* [Click here to launch the ratgdo ESP Web Tools flasher to install the firmware](flash.html)
* Buy [ratgdo shield only](https://square.link/u/xNP2Orez) $15
* Buy [ratgdo shield with ESP8266 D1 Clone](https://square.link/u/JaMwtjLL) $30

# About ratgdo 
ratgdo gives you **local** MQTT & dry contact control plus status feedback for your Chamberlain/LiftMaster Security+ 2.0 garage door opener. Security+ 2.0 uses an encrypted serial signal to control the door opener's open/close and light functions, which makes it impossible to use Shelly One, Go Control, Insteon I/O linc or any other dry contact relay device to control the door. 

ratgdo is a hardware shield (circuit board) & firmware for the ESP8266 based Wemos D1 Mini development board that wires to your door opener's terminals and restores dry contact control. It also allows you to control the door with MQTT over your local WiFi network which can be used to integrate direclty with NodeRED or Home Assistant, eliminating the need for another "smart" device. WiFi is **not** required if you wish to only use the dry contact interface.

ratgdo is *not* a cloud device and does *not* require a subscription service. The firmware is open source and free for anyone to use.

> **ratgdo shields are available for pre order**
> The first batch of shields are expected to be available by mid August 2022
>
> * [ratgdo shield only](https://square.link/u/xNP2Orez) $15
> * [ratgdo shield with ESP8266 D1 Clone](https://square.link/u/JaMwtjLL) $30


# ratgdo ESP Flasher
These instructions explain how to flash the ratgdo firmware onto your ESP8266 directly from your browser (Google Chrome or Chromium based browsers required).

## Installing the firmware
1. You must use Google Chrome or a Chromium based browser. No other browsers support serial device communication at this time.
2. Connect ESP8266 with USB cable. It's recommended to do this with the ESP8266 removed from the ratgdo shield.
3. [Click here to launch the ratgdo ESP Web Tools flasher](flash.html)
4. Choose the version you wish to install
  * **ratgdo** - includes WiFi configuration through ESP Tools. After flashing the firmware, ESP tools will ask you for your WiFi credentials. After the ESP8266 connects to your network, you will be given the option to _Vist Device_ so you can configure the MQTT settings.
  * **ratgdo (WiFi Disabled)** - If you do not wish to use WiFi or MQTT, choose the _WiFi Disabled_ version. With this version you can control the garage door with the ratgdo dry contacts only.

## Mounting Suggestion
The ratgdo sheld has a pair of holes in it which can be used to physically mount the board. The easiest method is to use a zip tie to mount the board to the slots in the translucent white light cover.

## Disclaimer
Use this device at your own risk.