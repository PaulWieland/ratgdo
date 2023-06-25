TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)

## Features
Version 2 of the shield supports detecting the garage door's position (opening, open, closing, closed) from the encrypted signal wire. No soldering or additional sensors are required to get the door status. Three simple wires (Ground, Signal and Obstruction) are connected to the terminal 

See [v1 Features](01_features_v1.md) if you have a an original ratgdo shield with a blue printed circuit board.

### MQTT

#### Home Assistant Auto Discovery
If you are using Home Assistant and have an MQTT broker setup, then HA will automatically discover ratgdo's door, light and obstruction sensors after it boots up.

When ratgdo boots up after being configured, it broadcasts the necessary discovery messages that tell Home Assistant what it is and how to communicate with it.

See [Home Assistant](05_homeassistant_example.md) for more information.

#### Triggers
The following MQTT commands are supported:

* door/open - opens the door.
* door/close - closes the door.
* light/on - turns the light on.
* light/off - turns the light off.
* lock/lock - locks out the wiresss remotes.
* lock/unlock - unlocks the use of wireless remotes.
* set_code_counter - sets the rolling code counter. Send a payload with an integer. Only needed to restore the rolling code counter if you completely erase the esp8266 flash memory.
* sync - syncs the rolling code counter with your door opener - must be called after <em>set_code_counter</em> (this also happens whenever rebooting the esp).

##### Examples

If:

* Device Name = "My Garage Door"
* mqtt Prefix = "home/garage"

Then:

* mqtt.topic = "home/garage/My Garage Door/command/door"; mqtt.payload = "open"; - opens the door
* mqtt.topic = "home/garage/My Garage Door/command/door"; mqtt.payload = "close"; - closes the door
* mqtt.topic = "home/garage/My Garage Door/set_code_counter"; mqtt.payload = 537; - sets the rolling code counter to 537
* mqtt.topic = "home/garage/My Garage Door/command"; mqtt.payload = "sync" - syncs the current rolling code counter with the garage door. 

<strong>Notice</strong> Older Security + 2.0 garage door openers with a logic board model# starting with 45 will not accept repeated rolling codes. You must use a rolling code value greater than the last one used with ratgdo. For this reason it is important to take note of the latest rolling code counter (displayed in the serial monitor output) whenever doing a full erase of the esp8266 flash storage.

#### Statuses
The following statuses are broadcast over MQTT:

* prefix/status/availability
 * online - once ratgdo connects to the MQTT broker.
 * offline - the mqtt last will message which is broadcast by the broker when the ratgdo client loses its connection.
* prefix/status/obstruction
 * obstructed - when an object breaks the obstruction sensor beam.
 * clear - when an obstruction is cleared.
* prefix/status/door
 * opening - when the door is opening.
 * open - when the door is fully open.
 * closing - when the door is closing.
 * closed - when the door is fully closed.
* prefix/status/light
 * on - when the light is on
 * off - when the light is off
* prefix/status/lock
 * locked - when the door opener is locked
 * unlocked - when unlocked

### Dry contacts

#### Triggers
The following dry contact triggers on the ratgdo shield are at 3.3v<sup>2</sup> and can be pulled to ground to trigger the door opener as follows:

* open<sup>1</sup> - opens the door.
* close<sup>1</sup> - closes the door.
* light - toggles the light on or off.

#### Statuses
The following dry contact statuses are available:

* door - pulled to ground if the door is open, open circuit if closed.
* obs - pulled to ground if the door is obstructed, open circuit if clear.


### Notes
1. <sup>1</sup> Repeated open commands (or repeated close commands) will be ignored. This gives discrete open/close control over the door which is better than a toggle.
1. <sup>2</sup> Using the ESP8266's internal INPUT_PULLUP resistor.
