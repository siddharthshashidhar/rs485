### ESP C3- node:
Acts as the mcu inside the battery pack
Sends data via the rs485 module

Pin config
    RX - 20
    TX - 21
Module id - 1

This node sends the voltage, current, temperature parameters of the battery to the mcu outside.

V1,V2,V3 - Voltage readigns of the three batteries
I - Current being drawn
t- temperature of the battery pack


### ESP32 - node:
recieves data and checks if the parameters are within the threshold

Pin config
    RX - 21
    TX - 19
Module id - 2

Checks if the voltage, current and temperature paramters are withing the safe threshold.

Also checks if there is an imbalance by comparaing the max and min voltage being drawn from the cells


Sends back 0 if it is safe or else 1



Every rs485 module has an id which is the first parameter sent along with the message it carries

Each time a message is recived, the time at which it was recived is recorcded.
If the time between two meesges recived from one module exceeds 3 seconds, it is flagged as damaged module

ESP C3 has a failsafe - even if the esp32 doesn't flag the temp brach or if it dies, esp c3 itself internall keeps track of the temperature and flags as soon as it breaches threshold