# Project description
TODO: Write an actual description

# Pin Allocation
![Nano Pins.jpeg](documentation%2FNano%20Pins.jpeg)

| Pin# | D Pin | Bank | Use          | Priority |
|------|-------|------|--------------|----------|
| 10   | D6    | PD5  | Motor PWM 1  | Medium   |
| 9    | D5    | PD6  | Motor PWM 2  | Medium   |


## Communication Protocols

## Sensors
### Ultrasonic Sensor
The Ultrasonic Sensor class was developed for the HC-SR04 sensor.

### IMU

## Actuators
### H-Bridge
#### PWM Control
The H-Bridge requires one PWM pin per side, and both sides should be ran at the same frequency, and thus on the same timer.
Timer0 was chosen.

### Servo


# Documentation
[Interrupts](documentation/interrupts.md)
