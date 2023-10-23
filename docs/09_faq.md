TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)


# FAQ & Troubleshooing

## Will ratgdo work with my garage door opener model?
Any residential door opener made in the last 25+ years by Chamberlain/Liftmaster should work. If your opener is mounted over head and has a **yellow** learn button, it is supported by ratgdo 2.0 or greater. If your opener is a wall mounted jackshaft opener or has a **red, purple or orange** learn button, it is supported by ratgdo 2.5 or greater.

If you have a *non* Chamberlain/LiftMaster opener that supports dry contact control, it is supported by ratgdo 2.5 **if**:

* the obstruction sensors operate at 5-6v, then ratgdo will be able to read them. A firmware patch may be required if they use a dramatically different feedback method than Chamberlain sensors.
	* If the sensors are not 5-6v, ratgdo will still be able to control the door but it will not provide obstruction status. CAUTION: connecting higher than 6v sensors will destroy the obstruction input circuit.
* you have limit switches which close to ground for sensing when the door is fully open AND fully closed.
	* some models provide user accessible terminals for the built in limit switches, otherwise you have to add your own external switches. A pair of inexpensive magnetic reed switchs is the best way to add these if you cannot use the door opener's built in switches.

## Which control protocol (Security + 1.0, Security + 2.0, dry contact, etc) do I need?

Door openers manufactured by the Chamberlain Group have a learn button on them. The color of the learn button can (mostly) tell you which control protocol the opener uses.

* Residential overhead mounted openers
	* with a yellow learn button are Security + 2.0
	* with a red, purple or orange learn button are Security + "1.0"
* Residential wall mounted jackshaft openers
	* With model 8500**W** are Security + 2.0
	* All others are Security + 1.0

* Security + 2.0 door openers require ratgdo v2.0 control board or later
* Security + 1.0 & Dry Contact door openers require v2.5 control board or later


Many non Chamberlain openers can be controlled with dry contacts, but also require the connection of limit switches to ratgdo in order for it to interpret the door positions (Closed, Opening, Open, Closing). See the [Wiring](03_wiring.md) section for an explanation of how to wire dry contact openers.

## ratgdo isn't able to control my door
The #1 most common mistake is with wiring. Double check your wiring compared to the diagram. The terminals are labeled on the **back** side of the board. From the front, the three control/ground/obstruction wires are on the right. When you flip the board over (the back has the QR code) they are on the left. The wiring diagram shows the back of the board (note the QR code) where the silkscreen is located.

## My PC can't connect with ratgdo
Make sure you have a USB to UART driver installed. If on windows, open your device manager and look to see if an unknown device shows up under the Ports -> USB. If on MacOS, open System Information.app and look under the USB Tree. An FTDI serial device should show up. The FTDI usb to serial or usb to uart driver should work. Modern OS's come with the driver pre-installed.

## My door opens/closes when ratgdo reboots
Version 2.0 (white PCB) ratgdo control boards are only compatible with Security + 2.0 openers. Wall mounted jackshaft openers (model 8500, 8500C and RJO20) are labeled as security + 2.0 but they use the older wireline protocol from the Security + 1.0 openers. If you have one of these openers and you purchased a V2.0 shield, please contact me for a replacement shield. 

If you own an opener with a purple or red learn button, you have a security + 1.0 opener and you need a v2.5 ratgdo control board.

<!-- ## Converting V1.x board to V2.0
Upgrading a V1 board to V2 requires the addition of a 2n7000 Mosfet.

Solder connections:
- source = ground
- gate = red/ctrl
- drain = RPM2 -->

[Please open an issue on GitHub for support](https://github.com/PaulWieland/ratgdo/issues). FAQs will be added here.
