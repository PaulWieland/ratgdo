TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)


# Troubleshooing & FAQ

## Will ratgdo work with my garage door opener model?
Any door opener with a wall control panel which has an X in the 2011+ column of [this chart](https://www.windsordoor.com/assets/uploads/modules/accessory-compatibility-chart-91817-82926.pdf) should work with ratgdo v2.

If you can't find your model in the chart, short your wall control panel wires together. If the door moves, then ratgdo will not work. A future release of ratgdo will work with the older control protocols.

## Converting V1.x board to V2.0
Upgrading a V1 board to V2 requires the addition of a 2n7000 Mosfet.

Solder connections:
- source = ground
- gate = red/ctrl
- drain = RPM2

[Please open an issue on GitHub for support](https://github.com/PaulWieland/ratgdo/issues). FAQs will be added here.
