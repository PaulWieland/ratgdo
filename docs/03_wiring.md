TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)


## Connecting the ratgdo v2.0 shield to your Security+ 2.0® garage door opener

Things you will need:

* A ratgdo shield **[Buy shield only](https://square.link/u/xNP2Orez)**
* A wemos D1 Mini pin compatible ESP8266 development board **[Buy shield + D1 mini Clone](https://square.link/u/JaMwtjLL)**
* A Micro USB cable
* A USB power brick
* A small flat blade screw driver for the ratgdo terminals
* A small zip tie for mounting the board

Procedure:

0. Make sure you have already [installed](flash.html) and [configured](02_configuration.md) the ratgdo 2.0 firmware.
1. Follow this diagram to connect your ratgdo 2.0 shield to your Security + 2.0 door opener <a href="https://user-images.githubusercontent.com/4663918/235453980-04a642fa-a181-4297-b4f3-06e1315e02fa.png">![image](https://user-images.githubusercontent.com/4663918/235453980-04a642fa-a181-4297-b4f3-06e1315e02fa.png)</a>
2. Power the ESP8266 with the USB power brick & cable.

## Version 1.0 Owners Only:
There are two ways you can connect an original ratgdo shield to your garage door opener.

* The first "Solder Free" method requires you to power ratgdo from a USB power brick and mount a reed switch for the door status.
  **[Solder Free Method &raquo;](03_wiring_solder_free.md)**

* The second "Solder" method uses the door opener's built in rotary encoder for detailed door status and also powers the ratgdo board.
  The soldered method requires you to solder three wires to the door opener's logic board. This method provides a couple of benefits.

  1. First, it allows ratgdo to know whether the garage door is opening, open, closing or closed. These statuses are broadcast out to the MQTT status topic.
  2. Second, no additional hardware such as a USB Charger, usb cable, or reed switch are needed.
  3. Last, the installation is cleaner looking.

  **[Solder Method &raquo;](03_wiring_soldered.md)**
