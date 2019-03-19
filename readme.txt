README:)

Just take this entire package and drop it into Arduino.

Before you can compile, read the following tutorials and install the following libraries for your sensors:

I gave the github link just in case, but you should be able to find the libraries in Arduino's builtin library manager.

1. INA219 Current Sensors (the blue ones. x2)
* Notice one of the sensors has the Address jumper soldered together (see tutorial). This makes the two sensors coexist by having different I2C addresses.
https://github.com/adafruit/Adafruit_INA219
https://learn.adafruit.com/adafruit-ina219-current-sensor-breakout

2. APDS9930 Light Sensor (the purple one.)
* Btw it measures so much more than ambient light. Feel free to play on your free time.
https://github.com/Depau/APDS9930

Also feel free to consult the example sketches provided by the two libraries. They're useful :)

Also don't forget to measure the voltage across the panel and battery. That's analogRead.
