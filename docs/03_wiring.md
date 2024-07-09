TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT / HomeKit Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)


## Connecting ratgdo to your garage door opener
0. Make sure you have already [installed](flash.html) and [configured](02_configuration.md) the ratgdo firmware.
1. Unplug the door opener.
1. Connect the wires according to the diagram below
1. Plug the door opener back in
1. Power ratgdo with the USB power brick & cable.

* For Security + 2.0 door openers within **North America**, and for all compatible Security + 1.0 openers, connect the red, white & black terminals:
	* v2.52 &amp; v2.53 Security + 1/2 diagram <a href="https://user-images.githubusercontent.com/4663918/288449523-9ddf3da2-9eac-4be0-beed-11867dc8d446.png">![image](https://user-images.githubusercontent.com/4663918/288449523-9ddf3da2-9eac-4be0-beed-11867dc8d446.png)</a>
	* v2.5  Security + 1/2 diagram <a href="https://user-images.githubusercontent.com/4663918/276749741-fe82ea10-e8f4-41d6-872f-55eec88d2aab.png">![image](https://user-images.githubusercontent.com/4663918/276749741-fe82ea10-e8f4-41d6-872f-55eec88d2aab.png)</a>
* If your Security + 2.0 door opener has an eserial terminal (common outside of North America) connect the red/ctrl wire to eserial and **not** the red terminal. Only connect the black obstruction sensor wire if you have obstruction sensors.

* For Dry Contact door openers, you will need reed switches for door position status:
	* v2.5x dry contact diagram <a href="images/dry_contact_diagram.png">![image](images/dry_contact_diagram.png)</a>


### Examples

Security + 1.0 & 2.0 openers using passthrough ports on control board (one wire per terminal):
v2.52 &amp; v2.53 (North America)
![image](images/sec2.0-wiring.jpg)

Rest of World Security + 2.0 Openers with e-serial terminal. Leave black obstruction wire disconnected unless you have obstruction sensors.
![image](images/sec2.0-row-wiring.jpg)

v2.5
![image](https://user-images.githubusercontent.com/4663918/278091586-63ff87f9-f67b-49a5-bc8a-c3c8a5bdab73.jpeg)
![image](https://user-images.githubusercontent.com/4663918/278091772-464265aa-a377-4ef8-b5d9-5b4bfb77d189.jpeg)

Security + 1.0 & 2.0 openers with multiple wires sharing a terminal on GDO:
![image](https://user-images.githubusercontent.com/4663918/278091799-a520915d-c475-4ac9-8b06-f9fca015f0c2.jpeg)


Example wiring for a Genie opener which has user accessible limit switch terminals:

* Green wire = wht/gnd -> terminal 2 / ground
* Orange wire = red/ctrl -> terminal 1 / control
* Blue wire = trigger open -> terminal 4 / open limit switch
* White wire = trigger close -> terminal 6 / close limit switch

![image](https://user-images.githubusercontent.com/4663918/278091841-2c0b9a98-649f-43c7-8506-f76a528d6972.jpeg)

![image](https://user-images.githubusercontent.com/4663918/278091855-f231ff4d-575b-4ab6-9263-7d590645e7f5.jpeg)