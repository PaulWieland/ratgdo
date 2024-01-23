TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT / HomeKit Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)

# Home Assistant Example
If you are using Home Assistant, there is an [ESPHome](http://github.com/ratgdo/esphome-ratgdo) version of ratgdo. This version is directly adoptable by your Home Assistant instance.

You can also integrate with Home Assitant through MQTT. Since version 1.7, ratgdo has home assistant auto discovery over MQTT. When ratgdo boots, three discovery messages are sent via mqtt which will add a door, light and obstruction sensor entities under a single device in Home Assistant.

There is also a link to the ratgdo configuration page from within the Home Assistant UI.

In the ratgdo configuration page, there is a place to change the Home Assistant auto discovery prefix. The default is `homeassistant` which is the what HA uses out of the box. You have to change this setting to match your Home Assistant's prefix if you have modified the standard value in HA's MQTT integration settings.