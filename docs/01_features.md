TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)

## Features
ratgdo detects the garage door's position (opening, open, closing, closed) from the encrypted signal wire. No soldering or additional sensors are required to get the door status. Three simple wires (Ground, Control and Obstruction) are connected to the terminals of the garage door opener.

See [v1 Features](01_features_v1.md) if you have a an original ratgdo with a blue printed circuit board.

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

##### Examples

If:

* Device Name = "MyGarageDoor"
* mqtt Prefix = "home/garage"

Then:

* mqtt.topic = "home/garage/MyGarageDoor/command/door"; mqtt.payload = "open"; - opens the door
* mqtt.topic = "home/garage/MyGarageDoor/command/door"; mqtt.payload = "close"; - closes the door


#### Statuses
The following statuses are broadcast over MQTT:

<ul>
	<li>refix/status/availability
		<ul>
			<li>online - once ratgdo connects to the MQTT broker.</li>
			<li>offline - the mqtt last will message which is broadcast by the broker when the ratgdo client loses its connection.</li>
		</ul>
	</li>
	<li>prefix/status/obstruction
		<ul>
			<li>obstructed - when an object breaks the obstruction sensor beam.</li>
			<li>clear - when an obstruction is cleared.</li>
		</ul>
	</li>
	<li>prefix/status/door
		<ul>
			<li>opening - when the door is opening.</li>
			<li>open - when the door is fully open.</li>
			<li>closing - when the door is closing.</li>
			<li>closed - when the door is fully closed.</li>
		</ul>
	</li>
	<li>prefix/status/light
		<ul>
			<li>on - when the light is on</li>
			<li>off - when the light is off</li>
		</ul>
	</li>
	<li>prefix/status/lock
		<ul>
			<li>locked - when the door opener is locked</li>
			<li>unlocked - when unlocked</li>
		</ul>
	</li>
</ul>

### ESPHome
There is an ESPHome port of ratgdo available. For the time being this port might not be feature compatible with the MQTT version of ratgdo.

* [Web Tools installer](https://ratgdo.github.io/esphome-ratgdo/)
* [GitHub Repo](https://github.com/ratgdo/esphome-ratgdo)


### Dry contacts

#### Triggers
ratgdo's dry contact triggers are at 3.3v and can be pulled to ground to trigger the door opener as follows:

* open<sup>1</sup> - opens the door.
* close<sup>1</sup> - closes the door.
* light - toggles the light on or off.

#### Statuses
The following dry contact statuses are available:

* door - pulled to ground if the door is open, open circuit if closed.
* obs - pulled to ground if the door is obstructed, open circuit if clear.


### Notes
1. <sup>1</sup> Repeated open commands (or repeated close commands) will be ignored. This gives discrete open/close control over the door which is better than a toggle.
