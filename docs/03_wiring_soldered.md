TOC
* [Main](index.md)
* [Features](01_features.md)
* [Configuration](02_configuration.md)
* [Wiring](03_wiring.md)
* [NodeRED MQTT Example](04_nodered_example.md)
* [Home Assistant Example](05_homeassistant_example.md)
* [FAQ & Troubleshooting](09_faq.md)

## Soldered method - uses door opener's rotary encoder and power supply

Soldering to the logic board is quite easy. These instructions are for a LiftMaster Model 8365W.

Things you will need:

* A ratgdo shield **[Buy shield only](https://square.link/u/xNP2Orez)**
* A wemos D1 Mini pin compatible ESP8266 development board **[Buy shield + D1 mini Clone](https://square.link/u/JaMwtjLL)**
* A soldering iron & basic soldering skills. Good solder (lead/tin works best) & flux.
* 3 pieces of thin gauge flexible wire, approximately 1' long (length depends on where you want to physically mount the ratgdo shield)
* A phillips screw driver or 1/4" nut driver
* A small flat blade screw driver for the ratgdo terminals
* Two small zip ties, one for strain relief of the wires and the other to mount the board

Procedure:

1. IMPORTANT: Unplug the door opener to physically disconnect power. **Failure to do so can kill you** as there is live main voltage present on the logic board.
2. Open the white plastic rear light cover where the terminals are for the obstruction sensor & hard wired remote.
3. Disconnect the wires from the obstruction sensor and hard wired button from the push terminals.
4. Remove the three 1/4"/Phillips drive screws that hold the black rear cover to the metal chassis. <br/> ![image](https://user-images.githubusercontent.com/4663918/177998230-01c9d0c8-9c0a-4a35-9aa5-9788a5cd697d.png)
5. The logic board is attached to this cover and will come out with the cover. There are two main cables connecting the logic board to the motor and rotary encoder. 
    * Optional - disconnect the two cable connectors if you want to completely remove the back cover and logic board for soldering.
6. Locate the RPM1, RPM2 and 3.3v pins on the logic board. See diagram(s).
7. Solder a wire to each of these pins. Make sure you keep track of which wire is which. Hooking them up to the ratgdo shield incorrectly will cause a malfunction.
8. Route the wires as shown in the picture. Use a small zip tie to provide strain relief.
9. Reconnect the logic board cables if you disconnected them in step 5, and reinstall the cover.
10. Connect the 6 wires from the door opener to the ratgdo shield according to this diagram:

<a href="https://user-images.githubusercontent.com/4663918/177997952-4e0f8ece-3309-4fb6-ab70-b2aa25bb092f.png">
![image](https://user-images.githubusercontent.com/4663918/177997952-4e0f8ece-3309-4fb6-ab70-b2aa25bb092f.png)
</a>

50ACCL1 Logic Board:<br/>
<a href="https://user-images.githubusercontent.com/4663918/177995941-b4989feb-de96-4f7a-a4cd-569aabcb7b94.png">
![image](https://user-images.githubusercontent.com/4663918/177995941-b4989feb-de96-4f7a-a4cd-569aabcb7b94.png)
</a>

45ACCL1 Logic Board:<br/>
<a href="https://user-images.githubusercontent.com/2976310/210608995-0f9814d4-0a85-490e-a789-d82b68cb7b88.jpg">
![image](https://user-images.githubusercontent.com/4663918/210647184-d28e5971-2dfa-4b5c-a147-2af85a5eda86.png)
</a>

