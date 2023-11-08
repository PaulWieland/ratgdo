TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT / HomeKit Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)

## Features
ratgdo detects the garage door's position (opening, open, closing, closed) from the encrypted signal wire. No soldering or additional sensors are required to get the door status. Three simple wires (Ground, Control and Obstruction) are connected to the terminals of the garage door opener.

See [v1 Features](01_features_v1.md) if you have a an original ratgdo with a blue printed circuit board.

## Feature Matrix
The features supported depend on the type of garage door opener you have and the firmware you are using. This table is meant to help clarify what features work with which opener.

Firmware types:

* <strong><a href="http://github.com/ratgdo/mqtt-ratgdo">M</a></strong>QTT - for MQTT based home automation integration (NodeRED, Home Assistant, etc)
* <strong><a href="http://github.com/ratgdo/esphome-ratgdo">E</a></strong>SP Home - for ESP Home / Home Assistant
* <strong><a href="http://github.com/ratgdo/homekit-ratgdo">H</a></strong>omeKit - (in development) if you just want iOS integration without the need for a home automation platform

* X - fully supported
* o - partial support, see note
* &plus; - coming soon

<table>
<thead>
	<tr>
		<th>&nbsp;</th>
		<th style="border-right: 1px solid #e5e5e5;" colspan="6">Manufactured by Chamberlain / Liftmaster</th>
		<th>Other</th>
	</tr>
	<tr>
		<th>&nbsp;</th>
		<th style="border-right: 1px solid #e5e5e5" colspan="3">Security + 2.0</th>
		<th style="border-right: 1px solid #e5e5e5" colspan="3">Security + 1.0</th>
		<th colspan="3">Dry Contact</th>
	</tr>
	<tr>
		<th style="width: 25%;">&nbsp;</th>
		<th style="border-right: 1px solid #e5e5e5; width: 25%" colspan="3">Yellow Learn Button</th>
		<th style="border-right: 1px solid #e5e5e5; width: 25%" colspan="3">Purple, Red, Orange Learn Buttons</th>
		<th style="width: 25%" colspan="3">&nbsp;</th>
	</tr>
	<tr>
		<th>Firmware</th>
		<th style="text-align: center;"><a href="http://github.com/ratgdo/mqtt-ratgdo">M</a></th>
		<th style="text-align: center;"><a href="http://github.com/ratgdo/esphome-ratgdo">E</a></th>
		<th style="text-align: center; border-right: 1px solid #e5e5e5"><a href="http://github.com/ratgdo/homekit-ratgdo">H</a></th>

		<th style="text-align: center;"><a href="http://github.com/ratgdo/mqtt-ratgdo">M</a></th>
		<th style="text-align: center;"><a href="http://github.com/ratgdo/esphome-ratgdo">E</a></th>
		<th style="text-align: center; border-right: 1px solid #e5e5e5"><a href="http://github.com/ratgdo/homekit-ratgdo">H</a></th>

		<th style="text-align: center;"><a href="http://github.com/ratgdo/mqtt-ratgdo">M</a></th>
		<th style="text-align: center;"><a href="http://github.com/ratgdo/esphome-ratgdo">E</a></th>
		<th style="text-align: center;"><a href="http://github.com/ratgdo/homekit-ratgdo">H</a></th>		
	</tr>
</thead>
<tbody>
	<tr>
		<th>Door Control</th>
		<td style="text-align: center;">X</td>
		<td style="text-align: center;">X</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">X</td>
		<td style="text-align: center;">+</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">X</td>
		<td style="text-align: center;">+</td>
		<td style="text-align: center;">+</td>

	</tr>
	<tr>
		<th>Door Status</th>
		<td style="text-align: center;">X</td>
		<td style="text-align: center;">X</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">X</td>
		<td style="text-align: center;">+</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">o<sup>1</sup></td>
		<td style="text-align: center;">+<sup>1</sup></td>
		<td style="text-align: center;">+<sup>1</sup></td>
	</tr>
	<tr>
		<th>Light Control</th>
		<td style="text-align: center;">X</td>
		<td style="text-align: center;">X</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">X</td>
		<td style="text-align: center;">+</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center;">&nbsp;</td>
	</tr>
	<tr>
		<th>Light Status</th>
		<td style="text-align: center;">X</td>
		<td style="text-align: center;">X</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">X</td>
		<td style="text-align: center;">+</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center;">&nbsp;</td>
	</tr>
	<tr>
		<th>Obstruction Status</th>
		<td style="text-align: center;">X</td>
		<td style="text-align: center;">X</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">X</td>
		<td style="text-align: center;">+</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">o<sup>2</sup></td>
		<td style="text-align: center;">+<sup>2</sup></td>
		<td style="text-align: center;">+<sup>2</sup></td>
	</tr>
	<tr>
		<th>Wireless Remote Lockout</th>
		<td style="text-align: center;">X</td>
		<td style="text-align: center;">X</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+</td>

		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">&nbsp;</td>

		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center;">&nbsp;</td>
	</tr>
	<tr>
		<th>Motion Detection</th>
		<td style="text-align: center;">o<sup>3</sup></td>
		<td style="text-align: center;">o<sup>3</sup></td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">+<sup>3</sup></td>

		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center; border-right: 1px solid #e5e5e5;">&nbsp;</td>

		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center;">&nbsp;</td>
		<td style="text-align: center;">&nbsp;</td>
	</tr>
</tbody>
</table>	

1. Openers with dry contact control require that limit switches be connected to ratgdo to detect the door state. See [Dry Contact Wiring](03_wiring.md).
1. Obstruction sensors must have a peak voltage between 4.5 and 7 volts.
1. Motion detection requires a wall control panel with a built in motion detector such as the 880LMW.


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
##### Chamberlain / Liftmaster openers
When using either Security + 1.0 or Security + 2.0 door opener, ratgdo's dry contact triggers can be pulled to ground to trigger the door opener as follows:

* open<sup>1</sup> - opens the door.
* close<sup>1</sup> - closes the door.
* light - toggles the light on or off.

##### Other openers
When using a door opener that supports standard dry contact control (door bell style), the trigger inputs are used to detect the door state.
For these openers connect as follows:

* trigger open terminal - wire to door open limit switch
* trigger close terminal - wire to door closed limit switch

Wire it in such a way that the trigger input is connected with ground when the limit switch is closed. Some door openers (e.g. Genie) have screw terminals on them for each limit switch in addition to the control terminal for opening the door. If your opener doesn't expose its internal limit switches to user accessible terminals you can add simple reed switches to the door track to detect it's state.

With these two limit switches connected, ratgdo can detect all four door states (closed, opening, open, closing).

#### Statuses
The following dry contact statuses are available:

* door - pulled to ground if the door is open, open circuit if closed.
* obs - pulled to ground if the door is obstructed, open circuit if clear.


### Notes
1. <sup>1</sup> Repeated open commands (or repeated close commands) will be ignored. This gives discrete open/close control over the door which is better than a toggle.
