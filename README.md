# ratgdo

ratgdo gives you **local** MQTT & dry contact control plus status feedback for your Chamberlain/LiftMaster Security+ 2.0 garage door opener. Security+ 2.0 uses an encrypted serial signal to control the door opener's open/close and light functions, which makes it impossible to use Shelly One, Go Control, Insteon I/O linc or any other dry contact relay device to control the door. 

ratgdo is a hardware shield (circuit board) & firmware for the ESP8266 based Wemos D1 Mini development board that wires to your door opener's terminals and restores dry contact control. It also allows you to control the door with MQTT over your local WiFi network which can be used to integrate direclty with NodeRED or Home Assistant, eliminating the need for another "smart" device. WiFi is **not** required if you wish to only use the dry contact interface.

ratgdo is *not* a cloud device and does *not* require a subscription service. The firmware is open source and free for anyone to use.

> **ratgdo shields are available for pre order**
> The first batch of shields are expected to be available by mid August 2022
>
> * [ratgdo shield only](https://square.link/u/xNP2Orez) $15
> * [ratgdo shield with ESP8266 D1 Clone](https://square.link/u/JaMwtjLL) $30

![image](https://user-images.githubusercontent.com/4663918/177624921-042e4da7-b284-43e8-84e4-b950a0d34840.png)


# Instructions

[Visit the github io page for instructions](https://paulwieland.github.io/ratgdo/).


# Special thanks

Special thanks to kosibar, Brad and TechJunkie01 - without whom this project would not have been possible.