# Arduino Controlled Coffee Roaster

Built around a brewpi arduino shield I had around. Needs the brewpi libraries.
Somewhere in the code tree you will find the code from lukeinator42's build, but the software is more or less 100% rewritten. 

Plans:
1 - add a mosfet to the fan to be able to pwm it down if very cold outside
2 - add thermometer to the beans as well

Dependencies

* adafruit thermocouple 31855
* adafruit pt100 31865
* brewpilibs
* fastio
* pid
