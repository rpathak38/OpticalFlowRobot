# ESP32 O-Flow
O-flow is an optical flow robot buitl upon the ESP32-Wrover-Dev module.
The robot tracks exact distances with the use of its optical flow sensor and can maintain straight paths despite motor/weight
irregularity.
The chassis was custom designed to fit the ESP32. Custom motor driver code was written for the ultra-low power DRV8833 in order to
greatly optimize battery usage and provide more accurate stop mechanisms.
The chassis also features a unique touch to wake up feature, where the ESP32 is pulled from deep sleep mode by touching any of the
frame supports.


### TODO
- Write custom motor control library.
- Figure out optical flow code.
- Figure capacitve touch wakeup.
- Figure out bluetooth sleep/arm.
- 