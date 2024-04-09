# Project description

TODO: Write an actual description

# Pin Allocation

![Nano Pins.jpeg](documentation%2FNano%20Pins.jpeg)

| Pin# | D Pin | Bank | Use                    | Priority |
|------|-------|------|------------------------|----------|
| 1    | D3    | PD3  | Motor PWM 1            | High     |
| 2    | D4    | PD4  | Motor Forward Dir      | Low      |
| 32   | D2    | PD2  | Motor Reverse Dir      | Low      |
| 9    | D5    | PD5  | Servo Control 1        | High     |
| 10   | D6    | PD6  | Servo Control 2        | High     |
| 11   | D7    | PD7  | Ultrasonic Trigger Pin | Medium   |
| 12   | D8    | PB0  | Ultrasonic Echo Pin 0  | Medium   |
| 13   | D9    | PB1  | Ultrasonic Echo Pin 1  | Medium   |
| 14   | D10   | PB2  | Ultrasonic Echo Pin 2  | Medium   |

## Communication Protocols

## Sensors

### Ultrasonic Sensor

The Ultrasonic Sensor class was developed for the HC-SR04 sensor. In it's current implementation it uses the timer1 for
interrupts.

### IMU

## Actuators

### H-Bridge

**TODO:** Add method to 'kick start' the motor, at low RPM motor does not startup properly, need way to be able to do
this

H-Bridge functionality added, with the ability to change the speed of a motor.

#### PWM Control

The H-Bridge requires one PWM pin per side, and both sides should be ran at the same frequency, and thus on the same
timer. Timer2 was chosen.

### Servo

Currently written in *wrong* way, and makes use of the timers themselves to simply update position of the servo. Can be
improved if using interrupts instead.

Is able to control 2 servos currently, on D5 & D6, without any interrupts to code.

# Documentation

[Interrupts](documentation/interrupts.md)
