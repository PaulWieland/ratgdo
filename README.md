# ratgdo

ratgdo gives you **local** MQTT & dry contact control plus status feedback for your residential Chamberlain/LiftMaster/Merlin garage door opener _including_ Security+ 2.0 openers. (Security+ 2.0 uses an obfuscated serial signal to control the door opener's open/close and light functions, which makes it impossible to use Shelly One, Go Control, Insteon I/O linc or any other dry contact relay device to control the door). 

ratgdo wires to your door opener's terminals and restores dry contact control. It also allows you to control the door with MQTT over your local WiFi network which can be used to integrate directly with NodeRED or Home Assistant, eliminating the need for another "smart" device.

ratgdo is *not* a cloud device and does *not* require a subscription service. The firmware is open source and free for anyone to use.

ratgdo also can control door openers that use a dry contact interface. Status feedback is provided through two reed switches.

> **ratgdo available to order**
> * [ratgdo v2.5 control board with installation kit](https://ratcloud.llc/products/ratgdo-v2-5i-kit) $45

# [Visit the github.io page for instructions](https://paulwieland.github.io/ratgdo/).
[ratgdo on GitHub.io](https://paulwieland.github.io/ratgdo/)

# Special thanks

Special thanks to kosibar, Brad and TechJunkie01 - without whom this project would not have been possible.
