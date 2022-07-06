TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [FAQ & Troubleshooting](09_faq.md)


## Connecting the ratgdo shield to your Security+ 2.0® garage door opener

There are two ways you can connect ratgdo to your garage door opener.

* The first "Solder Free" method requires you to power ratgdo from a USB power brick and mount a reed switch for the door status.
  **[Solder Free Method &raquo;](03_wiring_solder_free.md)**

* The second "Solder" method uses the door opener's built in rotary encoder for detailed door status and also powers the ratgdo board.
  The soldered method requires you to solder three wires to the door opener's logic board. This method provides a couple of benefits.

  1. First, it allows ratgdo to know whether the garage door is opening, open, closing or closed. These statuses are broadcast out to the MQTT status topic.
  2. Second, no additional hardware such as a USB Charger, usb cable, or reed switch are needed.
  3. Last, the installation is cleaner looking.

  **[Solder Method &raquo;](03_wiring_soldered.md)**
