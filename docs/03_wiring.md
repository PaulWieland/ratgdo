TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)


## Connecting ratgdo v2.0 or v2.5 to your garage door opener

Things you will need:

* A complete ratgdo install kit **[Buy here](https://square.link/u/FKqlMSWT)**

OR

* A ratgdo control board **[Buy here](https://square.link/u/B5pW7OZW)**
* A Micro USB cable
* A USB power brick
* 3 pieces of thin wire (22-26 gauge recommended)
* A small zip tie for mounting the board
* (v2.0 only) Small flat blade screw driver

Procedure:

0. Make sure you have already [installed](flash.html) and [configured](02_configuration.md) the ratgdo firmware.
1. Unplug the door opener.
1. Wiring between the v2.0 and v2.5 boards are very similar. v2.5 adds extra terminals to make wiring in parallel with your control panel and sensors easier.
	* v2.5 diagram Security + 1/2 diagram <a href="https://user-images.githubusercontent.com/4663918/276749741-fe82ea10-e8f4-41d6-872f-55eec88d2aab.png">![image](https://user-images.githubusercontent.com/4663918/276749741-fe82ea10-e8f4-41d6-872f-55eec88d2aab.png)</a>
	* v2.5 dry contact diagram <a href="https://user-images.githubusercontent.com/4663918/277838851-e338c3bf-4eda-447a-9e79-737aa1a622a0.png">![image](https://user-images.githubusercontent.com/4663918/277838851-e338c3bf-4eda-447a-9e79-737aa1a622a0.png)</a>
	* v2.0  diagram <a href="https://user-images.githubusercontent.com/4663918/235453980-04a642fa-a181-4297-b4f3-06e1315e02fa.png">![image](https://user-images.githubusercontent.com/4663918/235453980-04a642fa-a181-4297-b4f3-06e1315e02fa.png)</a>
2. Power ratgdo with the USB power brick & cable.
3. Plug the door opener back in.

## Version 1.0 Owners Only:
There are two ways you can connect an original ratgdo board to your garage door opener.

* The first "Solder Free" method requires you to power ratgdo from a USB power brick and mount a reed switch for the door status.
  **[Solder Free Method &raquo;](03_wiring_solder_free.md)**

* The second "Solder" method uses the door opener's built in rotary encoder for detailed door status and also powers the ratgdo board.
  The soldered method requires you to solder three wires to the door opener's logic board. This method provides a couple of benefits.

  1. First, it allows ratgdo to know whether the garage door is opening, open, closing or closed. These statuses are broadcast out to the MQTT status topic.
  2. Second, no additional hardware such as a USB Charger, usb cable, or reed switch are needed.
  3. Last, the installation is cleaner looking.

  **[Solder Method &raquo;](03_wiring_soldered.md)**
