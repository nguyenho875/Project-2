## Project 2: Enhanced Car Alarm and Automatic Headlight Control System
## Team Members: Hoang Tran and Nguyen Ho

Our project aims to enhance and implement two car subsystems, ignition alarm and automatic headlight control.

### Igniton Subsystem
The ignition subsystem should be able to welcome the driver once the seat is occupied, then check if both seats are filled and seatbelts fastened, all conditions of which are represented by buttons, to allow engine startup. If the car is ignited (by pressing the ignition button) when these conditions aren’t met, the system triggers an alarm and displays a message listing the violations. The serial terminal on the PC acts as the car’s display monitor,  and a buzzer is used as the vehicle’s alarm that sounds when an error occurs. 


### Headlight Subsystem
The driver controls the car's headlight using a mode selector represented by a potentiometer. There are 3 modes to the selector, ON to always turn on both headlights, OFF to always turn them off, and AUTO to turn them on and off depending on the brightness sensed by a light sensor. 


### Starting Repository
https://github.com/DwyerJd/Project1


### Testing results

| Specification | Test Result | 
|----------|----------|
| Ignition Subsystem | | |
| Enable engine start (i.e., light the green LED) while both seats are occupied and seatbelts fastened. Otherwise print appropriate error message.    | Pass   |
| Start the engine (i.e., light the blue LED, turn off Green) when ignition is enabled (green LED) and ignition button is pressed  (i.e., before the button is released).   | Pass   | |
| Keep the engine running even if the driver/passenger should unfasten belt and exit the vehicle.| Pass | |
| Turn on both LEDs when the mode selector (potentiometer) is turned to the ON mode. | Pass |
| | | |
|----------|----------|
| Automatic headlight control subsystem | | |
| Turn off both LEDs when the mode selector is turned to the OFF mode. | Pass |
| In AUTO mode, LEDs turn on after a 1 second delay when brightness is above “daylight” level, simulated by a phone flashlight placed 2 ft above the sensor. | Pass|
| In AUTO mode, LEDs turn off after a 2 second delay when brightness is below “dusk” level, simulated by holding a sheet of paper 6 in above the sensor | Pass |
| In AUTO mode, LEDS hold their previous state when the brightness is between “daylight” and “dusk” levels. | Pass |


