### ESP C3- node:
Sends the voltage, current, temperature parameters (here fixed) to the mcu outside the battery
Sends voltage and current every half second and temp data every 5 seconds

### ESP32 - node:
recieves data and checks if the parameters are within the threshold
Sends back 0 if it is safe or else 1

 the message sent includes id number of the the module which sends message
 the time at which the message was recived is noted and if the time exceeds more than 3seconds since it recived the last message the module is flagged as damaged

 esp c3 has a failsafe - it internally checks temperature never exceeds threshold even if the other modules have failed