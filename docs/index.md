# ORDER
### NEW WEBSITE NOTICE
New store launched on July 9th 2024: [http://ratcloud.llc](http://ratcloud.llc)

<!-- The original [square checkout link](https://square.link/u/FKqlMSWT) will remain active as we transition to the new site. -->
> * [ratgdo32 control board & accessories](https://ratcloud.llc/collections/frontpage)
> * [ratgdo holsters](https://ratcloud.llc/collections/holsters)


# What is ratgdo?
ratgdo is a WiFi control board for your garage door opener that works over your *local network* using ESPHome, HomeKit or MQTT.

It works with virtually any residential Chamberlain or Liftmaster manufactured garage door opener and also offers basic support for other brands which use dry contacts to control the door.

It can control the door and report back the actual status of the door (closed, opening, open, closing). For "yellow learn button" Security + 2.0 door openers, the obstruction sensors, light, wireless lockout and even motion sensing are also integrated.

[View the Feature Matrix for more information regarding which features & firmware work with which door opener.](01_features.md)

TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT / HomeKit Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)

# Quick Start
* [Firmware installation](flash.html)
  * If you are interested in using ESPHome check out the [ESPHome installer](http://ratgdo.github.io/esphome-ratgdo/).
  * The native HomeKit firmware allows you to adopt ratgdo into your Apple HomeKit environment without any central home automation server. [ratgdo for HomeKit installer](https://ratgdo.github.io/homekit-ratgdo)
* Wiring
  * [Version 2.52 &amp; v2.53 Security + 1/2 Wiring Diagram]()<br/><a href="https://user-images.githubusercontent.com/4663918/288449523-9ddf3da2-9eac-4be0-beed-11867dc8d446.png"><img src="https://user-images.githubusercontent.com/4663918/288449523-9ddf3da2-9eac-4be0-beed-11867dc8d446.png" alt="v2.52 & v2.53 wiring diagram" width="75"/></a>
  * [Version 2.52 &amp; v2.53 Dry Contact Wiring Diagram](https://user-images.githubusercontent.com/4663918/288450016-0caa5d03-325a-4c89-a144-a145ac907dfa.png)<br/><a href="https://user-images.githubusercontent.com/4663918/288450016-0caa5d03-325a-4c89-a144-a145ac907dfa.png"><img src="https://user-images.githubusercontent.com/4663918/288450016-0caa5d03-325a-4c89-a144-a145ac907dfa.png" alt="v2.52 &amp; v2.53 wiring diagram" width="75"/></a>
  * [Version 2.5 Security + 1/2 Wiring Diagram](https://user-images.githubusercontent.com/4663918/276749741-fe82ea10-e8f4-41d6-872f-55eec88d2aab.png) <br /><a href="https://user-images.githubusercontent.com/4663918/276749741-fe82ea10-e8f4-41d6-872f-55eec88d2aab.png"><img src="https://user-images.githubusercontent.com/4663918/276749741-fe82ea10-e8f4-41d6-872f-55eec88d2aab.png" alt="v2.5 wiring diagram" width="75"/></a>
  * [Version 2.5 Dry Contact Wiring Diagram](https://user-images.githubusercontent.com/4663918/277838851-e338c3bf-4eda-447a-9e79-737aa1a622a0.png)
  <br/><a href="https://user-images.githubusercontent.com/4663918/277838851-e338c3bf-4eda-447a-9e79-737aa1a622a0.png"><img src="https://user-images.githubusercontent.com/4663918/277838851-e338c3bf-4eda-447a-9e79-737aa1a622a0.png" alt="v2.5 dry contact diagram" width="75"/></a>


# About ratgdo 
ratgdo gives you **local** MQTT / ESPHome / HomeKit or dry contact control plus full status feedback for your Chamberlain/LiftMaster garage door opener. Security+ 2.0 uses an obfuscated serial signal to control the door opener's open/close and light functions, which makes it impossible to use Shelly One, Go Control, Insteon I/O linc or any other dry contact relay device to control the door. ratgdo supports virtually all residential garage door openers made by Chamberlain, and also supports other manufacturers through dry contact control.

ratgdo is a hardware control board that wires to your door opener's terminals. The firmware allows you to control the door with MQTT or ESPHome over your local WiFi network which can be used to integrate directly with NodeRED or Home Assistant, eliminating the need for another "smart" device. WiFi is **not** required if you wish to only use the dry contact interface.

ratgdo is *not* a cloud device and does *not* require a subscription service. The firmware is open source and free for anyone to use.

# ratgdo Firmware
* ESPHome - best for [Home Assistant](https://www.home-assistant.io), Chowmainsoft's [Control4](https://chowmain.software/drivers/control4-ratgdo), [Nice/Elan](https://chowmain.software/drivers/elan-ratgdo), &amp; [Crestron](https://chowmain.software/drivers/crestronhome-ratgdo) integrations, Finite Lab's [Control4](https://github.com/finitelabs/control4-esphome) integration, MQTT Integrations using a custom yaml
* HomeKit - best for those with iOS and without home automation platforms

## Version 2.5
Version 2.5 adds support for Security + 1.0 openers as well as standard dry contact openers. Extra wiring terminals for your wall control panel and obstruction sensors have been added which simplifies the installation. Security +1.0 requires an 889LM / 041A7928-3MC wall control panel. If you do not have an 889LM / 041A7928-3MC, ratgdo offers an emulation mode but it will interfere with your wall control's functions. dry contact control mode can be used with security +1.0 openers as a fall back. 

## Version 2.0
Version 2.0 eliminates the need to solder to your garage door's logic board. It works with all Security + 2.0 openers, including 45 series logic boards. 

Version 2.0 also provides discrete light on/off commands.


> **ratgdo v32 available to order**
> Boards are available and shipping domestic USA via USPS Ground Advantage.
> International shipments are sent via USPS Ground International
> Shipping to Canada is $17.
> Shipping to the EU and Austrailia varies between $15 and $24.
> International shipments are shipped under DDU incoterms.
>
> * [ratgdo v32 controller with installation kit](https://ratcloud.llc/products/ratgdo32)


# ratgdo ESP Flasher
Please see our website for complete & up to date instructions. [https://ratcloud.llc/pages/firmware](https://ratcloud.llc/pages/firmware)

These instructions explain how to flash the ratgdo firmware from your browser (Google Chrome or Chromium based browsers required).

## Installing the firmware
1. You must use Google Chrome or a Chromium based browser. No other browsers support serial device communication at this time.
2. Connect ratgdo to your Mac or PC with USB cable (Additional USB to UART drivers may be required). The blue LED will briefly flash and turn off when no firmware is present on the board.
3. [Click here to launch the ratgdo ESP Web Tools flasher](flash.html)
4. Choose the version you wish to install
  * **ratgdo** - includes WiFi configuration through ESP Tools. After flashing the firmware, ESP tools will ask you for your WiFi credentials. After ratgdo connects to your network, you will be given the option to _Visit Device_ so you can configure the MQTT settings.
  * **ratgdo (WiFi Disabled)** - If you do not wish to use WiFi or MQTT, choose the _WiFi Disabled_ version. With this version you can control the garage door with the ratgdo dry contacts only.

## Mounting
A holster for the ratgdo board is [available here](https://ratcloud.llc/products/ratgdo-v2-53i-mount). The holster is designed to clip onto the GDO's mounting frame or translucent light cover.

## Disclaimer
Use this device at your own risk. To avoid risk of injury or property damage only operate the garage door within line of sight.

ratgdo is a product of ratCloud llc and is not affiliated in anyway with the Chamberlain Group.

Security +, Security + 2.0, myQ are trademarks of the Chamberlain Group.

© 2023 ratCloud llc
