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

* A ratgdo (one per door)
* A Micro USB cable
* A USB power brick
* Reed switch
* A small flat blade screw driver for the ratgdo terminals
* A small zip tie for mounting the board

Procedure:

1. Mount a reed switch to the door track, and afix the magnet to the door in such a way that the reed switch will be activated when the door is fully open (or fully closed) and deactivated once the door moves.
2. Connect the reed switch (two wires) plus the door opener (three wires) to the ratgdo terminals according to this diagram: <a href="https://user-images.githubusercontent.com/4663918/233088336-6cbaeac2-e435-4758-9d90-fd0c62ff12f7.png">![image](https://user-images.githubusercontent.com/4663918/233088336-6cbaeac2-e435-4758-9d90-fd0c62ff12f7.png)</a>

2. Power ratgdo with the USB power brick & cable


