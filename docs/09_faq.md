TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT / HomeKit Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)


# FAQ & Troubleshooing

## Will ratgdo work with my garage door opener model?
Any residential door opener made in the last 25+ years by Chamberlain/Liftmaster should work. If your opener is mounted over head and has a **yellow** learn button, it is supported by ratgdo hardware v2.0 or greater. If your opener is a wall mounted jackshaft opener or has a **red, purple or orange** learn button, it is supported by ratgdo hardware v2.5 or greater.

There is also a [wiki page](https://github.com/PaulWieland/ratgdo/wiki) to document models which have been tested.

If you have a *non* Chamberlain/LiftMaster opener that supports dry contact control, it is supported by ratgdo 2.5 **if**:

* the obstruction sensors operate at 5-7v, then ratgdo will be able to read them. A firmware patch may be required if they use a dramatically different feedback method than Chamberlain sensors.
	* If the sensors are not 5-7v, ratgdo will still be able to control the door but it will not provide obstruction status. CAUTION: connecting higher than 6v sensors will destroy the obstruction input circuit.
* you have limit switches which close to ground for sensing when the door is fully open AND fully closed.
	* some models provide user accessible terminals for the built in limit switches, otherwise you have to add your own external switches. A pair of inexpensive magnetic reed switchs is the best way to add these if you cannot use the door opener's built in switches.

## Which control protocol (Security + 1.0, Security + 2.0, dry contact, etc) do I need?

A matrix of compatible openers is being matinained in the [ratgdo wiki on github](https://github.com/PaulWieland/ratgdo/wiki).

Door openers manufactured by the Chamberlain Group have a learn button on them. The color of the learn button can (mostly) tell you which control protocol the opener uses.

* Residential overhead mounted openers
	* with a yellow learn button are Security + 2.0
	* with a red, purple or orange learn button are Security + "1.0"
* Residential wall mounted jackshaft openers
	* With model 8500**W** or RJ070 are Security + 2.0
	* All others are Security + 1.0

* Security + 2.0 door openers require ratgdo v2.0 control board or later
* Security + 1.0 & Dry Contact door openers require v2.5 control board or later


Many non Chamberlain openers can be controlled with dry contacts, but also require the connection of limit switches to ratgdo in order for it to interpret the door positions (Closed, Opening, Open, Closing). See the [Wiring](03_wiring.md) section for an explanation of how to wire dry contact openers.

## ratgdo isn't able to control my door
The #1 most common mistake is with wiring. Double check your wiring compared to the diagram. The terminals are labeled on the **back** side of the board. From the front, the three control/ground/obstruction wires are on the right. When you flip the board over (the back has the QR code) they are on the left. The wiring diagram shows the back of the board (note the QR code) where the silkscreen is located.

## Drivers?
If you can't connect to your ratgdo board make sure you have the right driver installed for the type of board you have.

<ul>
	<li>ratgdo v2.5i uses a CH340 USB to Serial chipset. [<a href="https://www.wch-ic.com/downloads/CH341SER_EXE.html" target="_blank">driver download</a>]</li>
	<li>Most D1 Minis use an FTDI USB to Serial chipset. [<a href="https://ftdichip.com/drivers/vcp-drivers/" target="_blank">driver download</a>]</li>
</ul>

## My PC can't connect with ratgdo
The most common problem is a bad USB cable. Unfortunately many USB cables are power only (for charging a device) and do not include the data wires. Make sure you are using a USB cable that is an actual data cable.

Make sure you have a USB to UART driver installed. If on windows, open your device manager and look to see if an unknown device shows up under the Ports -> USB. If on MacOS, open System Information.app and look under the USB Tree. An FTDI serial device should show up. The FTDI usb to serial or usb to uart driver should work. Modern OS's come with the driver pre-installed.

## I have more than one door, do I need more than one ratgdo?
Yes. You need one ratgdo control board per door, just like you need one wired wall button per door.

## After hooking up ratgdo the range of my wireless remotes seems to be reduced
Some people have reported that the USB power bricks they used were generating noise that was interfering with the garage door opener's wireless signal. By swapping to a different USB brick, the problem was cured.

ratgdo operates on 2.4GHz WiFi while most wireless garage door openers operate at 310MHz, so they will not interfere with each other. However, signal attenuation can occur if wires or physical objects are touching or near the GDO's antenna. Make sure the GDO's antenna (just a loose piece of wire) is hanging down from the opener and that the ratgdo control board (and wiring) is at least 6" away from it.

## My door opens/closes when ratgdo reboots
Version 2.0 (white PCB) ratgdo control boards are only compatible with Security + 2.0 openers. Wall mounted jackshaft openers (model 8500, 8500C and RJO20) are labeled as security + 2.0 and have a yellow learn button, but they use the older wireline protocol from the Security + 1.0 openers. If you have one of these openers and you purchased a V2.0 shield, please contact me for a replacement shield. 

If you own one of the aformentioned jack shaft openers, or an opener with a purple, red or orange learn button, you have a security + 1.0 opener and you need a v2.5 (or later) ratgdo control board.

## Which version of the board will I receive?
Due to high demand and part availabilty constraints I have multiple manufacturers producing ratgdo control boards in the 2.5x range. **My main goal is to get the entire backlog of orders taken care of as fast as I can.** Orders are being shipped out in the order they are received using the stock that arrives first. All control boards starting with v2.5 are functionally identical but physical differences are present due to availability of parts and manufacturing process. One revision of 2.5x is not better than another.

## Do I need a case?
No. A plastic case for a low voltage device like ratgdo is completely unecessary and will only generate plastic waste [which stands a very low chance of being recycled](https://www.google.com/search?q=what+percentage+of+plastic+is+recycled). ratgdo is going to get installed near your garage door opener and then forgotten. The suggested mounting method is to zip tie it to the plastic light housing cover (if you have one) so that the terminals do not touch any metal. It will then sit invisible doing its job.


If you have a question about your order, you must reply to your square receipt.

[Please send an email to ratgdo [a] gmail for support]. FAQs will be added here.
