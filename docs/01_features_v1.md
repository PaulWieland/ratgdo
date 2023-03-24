TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)

## Features
The door opener has a rotary encoder built into it which allows the opener to know the exact position of the door. By soldering two wires to the logic board, ratgdo can monitor this rotary encoder so that it knows the exact state of the door.

For those that do not want to solder, there is an option to use an external reed switch which will give a less detailed binary status of either reed_open or reed_closed. See *[Wiring](03_wiring.md)* section.

### MQTT
#### Triggers
The following MQTT commands are supported:

* open<sup>2</sup> - opens the door.
* close<sup>2</sup> - closes the door.
* light<sup>3</sup> - toggles the light on or off.
* set_code_counter - sets the rolling code counter. Send a payload with an integer.
* sync - syncs the rolling code counter with your door opener - must be called after <em>set_code_counter</em> (this also happens whenever rebooting the esp).

##### Examples

If:

* Device Name = "My Garage Door"
* mqtt Prefix = "home/garage"

Then:

* mqtt.topic = "home/garage/My Garage Door/command/open" - opens the door
* mqtt.topic = "home/garage/My Garage Door/command/close" - closes the door
* mqtt.topic = "home/garage/My Garage Door/command/set_code_counter"; mqtt.payload = 537; - sets the rolling code counter to 537
* mqtt.topic = "home/garage/My Garage Door/command/sync" - syncs the current rolling code counter with the garage door. 

<strong>Notice</strong> Older Security + 2.0 garage door openers with a logic board model# starting with 45 will not accept repeated rolling codes. You must use a rolling code value greater than the last one used with ratgdo. For this reason it is important to take note of the latest rolling code counter (displayed in the serial monitor output) whenever doing a full erase of the esp8266 flash storage.

#### Statuses
The following statuses are broadcast over MQTT:

* online - once ratgdo connects to the MQTT broker.
* offline - the mqtt last will message which is broadcast by the broker when the ratgdo client loses its connection.
* obstructed - when an object breaks the obstruction sensor beam.
* clear - when an obstruction is cleared.
* opening<sup>1</sup> - when the door is opening.
* open<sup>1</sup> - when the door is fully open.
* closing<sup>1</sup> - when the door is closing.
* closed<sup>1</sup> - when the door is fully closed.
* reed_closed<sup>5</sup> - the reed switch is open (magnet is near the switch).
* reed_open<sup>5</sup> - the reed switch is closed (magnet is not near the switch).


### Dry contacts

#### Triggers
The following dry contact triggers on the ratgdo shield are at 3.3v<sup>4</sup> and can be pulled to ground to trigger the door opener as follows:

* open<sup>2</sup> - opens the door.
* close<sup>2</sup> - closes the door.
* light<sup>3</sup> - toggles the light on or off.

If you do not want to use the discrete open/close dry contacts and wish to have a toggle instead, then simply connect your switch/relay to both the open & close terminals on the ratgdo shield.

#### Statuses
The following dry contact statuses are available:

* door - 
  * When using a reed switch: 3.3v if the reed switch is closed, 0v if the reed switch is open.
  * When using the door's rotary encoder: 3.3v if the door is open, 0v if the door is closed.
* obs - 3.3v if an obstruction is detected. 0v if no obstruction.


### Notes
1. <sup>1</sup> Only when using the door opener's rotary encoder, which requires soldering to the logic board. See _Soldered Method_ below.
1. <sup>2</sup> Repeated open commands (or repeated close commands) will be ignored when using the rotary encoder. This gives discrete open/close control over the door which is better than a toggle. This is only reliable when ratgdo knows the doors position, which is only possible when connecting it to the door opener's rotary encoder.
1. <sup>3</sup> Unfortunately the door opener does not support discrete on/off commands for controlling the light, so the best we can do is toggle it.
1. <sup>4</sup> Using the ESP8266's internal INPUT_PULLUP resistor.
1. <sup>5</sup> Not used when using the door opener's rotary encoder.