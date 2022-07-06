TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)

## No Solder method - external power supply and reed switch
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