MQTT RF Bridge
==============

Use a CC3200 launchpad to bridge MQTT and cheap RF power outlets. [More information on my blog](TODO).


# Wiring diagram
- Launchpad 5V: RF module VCC
- Launchpad GND: RF module GND
- Replace D7 with a ~1K resistor, connect collector of the transistor above D7 to the RF module's data line
