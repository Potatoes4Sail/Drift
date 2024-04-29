# Project description

TODO: Write an actual description
[printer.cfg](..%2F..%2F..%2F..%2FDocuments%2FTrident%2FInitial%20Configs%2F2024-04-28%2Fprinter_data%2Fconfig%2Fprinter.cfg)
# Pin Allocation

![Nano Pins.jpeg](documentation%2FNano%20Pins.jpeg)

| Pin# | D Pin | Bank | Use                    | Priority |
|------|-------|------|------------------------|----------|
| 30   | RX0   | PD0  | IBus reciever          | High     |
| 1    | D3    | PD3  | Motor PWM 1            | High     |
| 1    | D11   | PB3  | Motor PWM 1            | High     |     ADDED
| 2    | D4    | PD4  | Motor Forward Dir      | Low      |
| 32   | D2    | PD2  | Motor Reverse Dir      | Low      |
| 9    | D5    | PD5  | Servo Control 1        | High     |
| 11   | D7    | PD7  | Ultrasonic Trigger Pin | Medium   |
| 12   | D8    | PB0  | Ultrasonic Echo Pin 0  | Medium   |
| 13   | D9    | PB1  | Ultrasonic Echo Pin 1  | Medium   |
| 14   | D10   | PB2  | Ultrasonic Echo Pin 2  | Medium   |
| 23   | D14   | PC0  | Motor Encoder A        | Medium   |
| 24   | D15   | PC1  | Motor Encoder B        | Medium   |
| 25   | D16   | PC2  | Left Wheel Encoder A   | Medium   |
| 26   | D17   | PC3  | Left Wheel Encoder B   | Medium   |
| 27   | D18   | PC4  | Right Wheel Encoder A  | Medium   |
| 28   | D19   | PC5  | Right Wheel Encoder B  | Medium   |

[//]: # (| 10   | D6    | PD6  | Servo Control 2        | High     |)

# Timer allocation:

Timer0 - Left for Arduino functions (micros & millis)
Timer1 - Use for timing durations of echo pulses
Timer2 - Used for motor & servo PWM control.

## Communication Protocols

## Sensors

### Ultrasonic Sensor

The Ultrasonic Sensor class was developed for the HC-SR04 sensor. In it's current implementation it uses the timer1 for
interrupts.

Echo pins are set on B bank of IO pins. All the pins will need to be on this bank to be able to use the interrupt
features

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
