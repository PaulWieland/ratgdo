# ORDER
### LEAD TIME NOTICE:
Please allow 2-3 weeks for shipment.
This increased lead time is due to the necessity of issuing a hardware revision (v2.0 to v2.5). 
Lead times will continue to improve.

> * [ratgdo v2.5 control board only](https://square.link/u/B5pW7OZW) $30
> * [ratgdo v2.5 control board with installation kit](https://square.link/u/FKqlMSWT) $42

TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)

# Quick Start
* [Click here to launch the ratgdo ESP Web Tools flasher to install the firmware](flash.html)
* A port of ratgdo to ESPHome exists. If you are interested in using ESPHome instead of MQTT check out the [ESPHome installer](http://ratgdo.github.io/esphome-ratgdo/) & [GitHub Repo](https://github.com/ratgdo/esphome-ratgdo).
* [Version 2.5 Wiring Diagram](https://user-images.githubusercontent.com/4663918/276749741-fe82ea10-e8f4-41d6-872f-55eec88d2aab.png) <br /><a href="https://user-images.githubusercontent.com/4663918/276749741-fe82ea10-e8f4-41d6-872f-55eec88d2aab.png"><img src="https://user-images.githubusercontent.com/4663918/276749741-fe82ea10-e8f4-41d6-872f-55eec88d2aab.png" alt="v2.5 wiring diagram" width="75"/></a>
* [Version 2.0 Wiring Diagram](https://user-images.githubusercontent.com/4663918/235453980-04a642fa-a181-4297-b4f3-06e1315e02fa.png) <br/><a href="https://user-images.githubusercontent.com/4663918/235453980-04a642fa-a181-4297-b4f3-06e1315e02fa.png"><img src="https://user-images.githubusercontent.com/4663918/235453980-04a642fa-a181-4297-b4f3-06e1315e02fa.png" alt="v2.0 wiring diagram" width="75"/></a>

# About ratgdo 
ratgdo gives you **local** MQTT / ESPHome & dry contact control plus full status feedback for your Chamberlain/LiftMaster garage door opener. Security+ 2.0 uses an encrypted serial signal to control the door opener's open/close and light functions, which makes it impossible to use Shelly One, Go Control, Insteon I/O linc or any other dry contact relay device to control the door. ratgdo supports virtually all residential garage door openers made by Chamberlain, and also supports other manufacturers through dry contact control.

ratgdo is a hardware control board that wires to your door opener's terminals. The firmware allows you to control the door with MQTT or ESPHome over your local WiFi network which can be used to integrate directly with NodeRED or Home Assistant, eliminating the need for another "smart" device. WiFi is **not** required if you wish to only use the dry contact interface.

ratgdo is *not* a cloud device and does *not* require a subscription service. The firmware is open source and free for anyone to use.

## Version 2.5
Version 2.5 adds support for Security + 1.0 openers as well as standard dry contact openers. Extra wiring terminals for your wall control panel and obstruction sensors have been added which simplifies the installation. 

## Version 2.0
Version 2.0 eliminates the need to solder to your garage door's logic board. It works with all Security + 2.0 openers, including 45 series logic boards. 

Version 2.0 also provides discrete light on/off commands.



> **ratgdo v2.5 available to order**
> Boards are available and shipping domestic USA via USPS.
> Shipping to Canada is also available, but costs $14.
> Shipping to the EU and Austrailia varies between $15 and $20.
>
> * [ratgdo v2.5 controller only](https://square.link/u/B5pW7OZW) $30
> * [ratgdo v2.5 controller with installation kit](https://square.link/u/FKqlMSWT) $42

![image](https://user-images.githubusercontent.com/4663918/177624921-042e4da7-b284-43e8-84e4-b950a0d34840.png)


# ratgdo ESP Flasher
These instructions explain how to flash the ratgdo firmware from your browser (Google Chrome or Chromium based browsers required).

## Installing the firmware
1. You must use Google Chrome or a Chromium based browser. No other browsers support serial device communication at this time.
2. Connect ratgdo to your Mac or PC with USB cable (Additional USB to UART drivers may be required).
3. [Click here to launch the ratgdo ESP Web Tools flasher](flash.html)
4. Choose the version you wish to install
  * **ratgdo** - includes WiFi configuration through ESP Tools. After flashing the firmware, ESP tools will ask you for your WiFi credentials. After ratgdo connects to your network, you will be given the option to _Visit Device_ so you can configure the MQTT settings.
  * **ratgdo (WiFi Disabled)** - If you do not wish to use WiFi or MQTT, choose the _WiFi Disabled_ version. With this version you can control the garage door with the ratgdo dry contacts only.

## Mounting Suggestion
The ratgdo board has a pair of holes in it which can be used to physically mount the board. The easiest method is to use a zip tie to mount the board to the slots in the translucent white light cover. A mounting exclosure for v2.0 by samwiseg0 can be found on [printables](https://www.printables.com/model/602600-ratdgo-v2-pcb-enclosure).

## Disclaimer
Use this device at your own risk.


ratgdo is a product of ratCloud llc and is not affiliated in anyway with the Chamberlain Group.

Security +, Security + 2.0, myQ are trademarks of the Chamberlain Group.

© 2023 ratCloud llc
