TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT / HomeKit Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)


# FAQ & Troubleshooing

## When will my order ship?
I'm working as fast as I can to ship orders while also dealing with a large volume of email inquiries asking when orders will ship. This takes away from the time I have available to support other questions and actually ship units once I receive them. In an effort to inform everyone of what's going on with their order, I created this [order status page](order_status.html) which I will update once a week.

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

Many non Chamberlain openers can be controlled with dry contacts, but also require the connection of limit switches to ratgdo in order for it to interpret the door positions (Closed, Opening, Open, Closing). See the [Wiring](03_wiring.md) section for an explanation of how to wire dry contact openers.

## ratgdo isn't able to control my door
The #1 most common mistake is with wiring. Double check your wiring compared to the diagram. The terminals are labeled on the **back** side of the board. From the front, the three control/ground/obstruction wires are on the right. When you flip the board over (the back has the QR code) they are on the left. The wiring diagram shows the back of the board (note the QR code) where the silkscreen is located.

## ratgdo is wired correctly and won't control my door
Verify that you are using the correct control protocol for your door opener model. 

Yellow Learn button openers (except for the jack shaft wall mounted 8500/RJ020 8500C/RJ020C) use Security + 2.0. Purple / orange and some red learn button openers use Security + 1.0. Everything else uses dry contact control.

If you are using Security + 2.0, you can try and reinstall the firmware and choose the "Erase" option which will force the generation of a new client ID for communicating with the door.

## ratgdo can't detect the door status of my Security + 1.0 opener
Security + 1.0 openers require that the wall control panel ask for the door opener for it's status. ratgdo listens to this communication and reports what it hears. myQ branded wall panels should all query for the door status. Non myQ wall panels have no reason to query for door status. These "dumb" wall panels fall into two categories: digital or analog.

Analog wall panels (such as the 78LM) do not communicate with the door opener at all, and in this case ratgdo is able to query the garage door opener for status directly. This allows ratgdo to know the door state, but it has the side effect of disabling the analog wall panel's light and lock buttons (the door open button still works normally).

Non myQ digital wall panels use digitial communication to control the door opener, but they do not ask the door opener for it's status and so ratgdo has no way to know what the door status is. If you have one of these wall panels then there are two options:

1. Use ratgdo's dry contact control and reed switches to get the door status
2. Buy either an Analog wall panel (a simple door bell/momentary contact switch will work), or a digital one which is myQ compatible such as the 889LM / 041A7928-3MC, which queries the door for it's state.

## My Security + 1.0 wall panel's light and lock buttons don't work after connecting ratgdo
You have an analog control wall panel. ratgdo in Security + 1.0 control mode requires a digital communication with the garage door opener in order to both control the door and to get the door's status. When the door opener is in digital communication mode, the analog light and lock controls are ignored.

If this is a deal breaker for you, you have two choices:

1. Use ratgdo's dry contact control and reed switches to get the door status
2. Buy a digital wall control which is myQ compatible such as the 889LM / 041A7928-3MC.


## Drivers?
If you can't connect to your ratgdo board make sure you have the right driver installed for the type of board you have.

<ul>
	<li>ratgdo v2.5i uses a CH340 USB to Serial chipset. [<a href="https://www.wch-ic.com/downloads/CH341SER_EXE.html" target="_blank">driver download</a>]</li>
	<li>Most D1 Minis use an FTDI USB to Serial chipset. [<a href="https://ftdichip.com/drivers/vcp-drivers/" target="_blank">driver download</a>]</li>
</ul>

## My PC can't connect with ratgdo
The most common problem is a bad USB cable. Unfortunately many USB cables are power only (for charging a device) and do not include the data wires. Some data cables can simply go bad from being flexed too many times. Make sure you are using a USB cable that is an actual data cable and if possible try another cable or device to rule out the cable.

The second most common problem are USB hubs. Plug your USB cable directly into a port on your computer.

Make sure you have a USB to UART driver installed. If on windows, open your device manager and look to see if an unknown device shows up under the Ports -> USB. If on MacOS, open System Information.app and look under the USB Tree. A serial device should show up. ratgdo v2.5i, v2.51i, v2.52i & v2.53i use the CH40 USB to Uart driver.

Here is a walkthrough video explaining how to install the drivers and flash the firmware.

<iframe width="640" height="360" src="https://youtube.com/embed/9WecAUTC9iI"></iframe>


## I have more than one door, do I need more than one ratgdo?
Yes. You need one ratgdo control board per door, just like you need one wired wall button per door.

## After hooking up ratgdo the range of my wireless remotes seems to be reduced
ratgdo operates on 2.4GHz WiFi while most wireless garage door openers operate at 310MHz, so they will not interfere with each other. However, signal attenuation can occur if wires or physical objects are touching or near the GDO's antenna. Make sure the GDO's antenna (just a loose piece of wire) is hanging down from the opener and that the ratgdo control board (and wiring) is as far away from it as practical.

## Which version of the board will I receive?
Due to high demand and part availabilty constraints I have multiple manufacturers producing ratgdo control boards in the 2.5x range. **My main goal is to get the entire backlog of orders taken care of as fast as I can.** Orders are being shipped out in the order they are received using the stock that arrives first. All control boards starting with v2.5 are functionally identical but physical differences are present due to availability of parts and manufacturing process. One revision of 2.5x is not better than another.

## Do I need a case?
No. A plastic case for a low voltage device like ratgdo is completely unecessary and will only generate plastic waste [which stands a very low chance of being recycled](https://www.google.com/search?q=what+percentage+of+plastic+is+recycled). ratgdo is going to get installed near your garage door opener and then forgotten. The suggested mounting method is to zip tie it to the plastic light housing cover (if you have one) so that the terminals do not touch any metal. It will then sit invisible doing its job.


If you have a question about your order, you must reply to your square receipt.

[Please send an email to ratgdo [a] gmail for support]. FAQs will be added here.
