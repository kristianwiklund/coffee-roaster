# Arduino Controlled Coffee Roaster

Dependencies

* adafruit thermocouple 31855
* adafruit pt100 31865
* brewpilibs 
* fastio
* pid

If you have arduino-cli installed you can install all dependencies by a simple "make arduino-deps"

# Physical design

The device is constructed from a popcorn maker. The heating element is connected to mains through a solid state relay, which is controlled by an arduino.
The arduino runs a simple PID, and reads temperature from the hot air below the roasting (popping) chamber using a thermocouple.
The fan is powered from a 19V HP laptop charger partially because I managed to blow up the series resistor, partially because I wanted to get rid of the 1:1 connection between heating and fan.

Since I happened to have a spare brewpi arduino shield lying around, the LCD and the decoder is connected to the arduino through the brewpi, but any display / encoder should work.

The SSR is connected to arduino pin 5.

# Other notes

Mosfet board schematics: https://robojax.com/learn/arduino/?vid=robojax-IRF520-MOSFET
