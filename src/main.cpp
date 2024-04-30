#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ultrasonic.h"
#include "pinDefinition.h"
#include "helperFunctions.h"
#include "L298Driver.h"
#include "servoDriver.h"
#include "pinDefinition.h"
#include "testCases.h"
#include "encoders.h"
#include "bst7960Driver.h"
#include <IBusBM.h>

// Global variables
Ultrasonic ultrasonicSensors = Ultrasonic(ULTRASONIC_SENSOR_TRIGGER_PIN,
                                          ULTRASONIC_SENSOR0_ECHO_PIN,
                                          ULTRASONIC_SENSOR1_ECHO_PIN,
                                          ULTRASONIC_SENSOR2_ECHO_PIN);
encoders wheelEncoders = encoders();

L298Driver oldMotor(3, 4, 2, 15);
BST7960Driver motor(MOTOR_PIN_PWM_FORWARD, MOTOR_PIN_PWM_REVERSE, 15);

IBusBM IBus; // IBus object

void stopAll();

void manualControlFunc(int &oldServoVal, int &oldMotorVal, int servoVal, int motorVal);

int main() {
    init(); // Needed for arduino functionality
//    Serial.begin(115200);
    IBus.begin(Serial);   // first instance - Uses compA of Timer0,
    servoDriverInit();
    customInitialization(); // Needed for initializing the timers.

    Serial.println("Start IBus2PWM");

    volatile uint64_t i = 0;

    int oldServoVal = 0;
    int oldMotorVal = 0;

    int servoVal;
    int motorVal;

    int autonomousSpeedChannel = 0, cruiseControlEnable = 0, autonomousEnableChannel = 0, channel5 = 0, tractionControl = 0, channel9 = 0;
    unsigned long encoderPrintTime = 0;
    bool manualControl = true;

    while (true) {

        _delay_ms(100);

        // ================================================
        //
        //          READING IBUS INPUTS FROM RC.
        //
        // ================================================
        motorVal = IBus.readChannel(1);                    // get latest value for servo channel 2 (right vertical)
        servoVal = IBus.readChannel(3);                    // get latest value for servo channel 4 (left horizontal)
        autonomousSpeedChannel = IBus.readChannel(4);           // VRA - Adjust auto speed value?
        channel5 = IBus.readChannel(5);                         // VRB - Unused
        autonomousEnableChannel = IBus.readChannel(6);   // SWA - also disables all inputs - use for full autonomy
        cruiseControlEnable = IBus.readChannel(7);
        tractionControl = IBus.readChannel(8);                  //
        channel9 = IBus.readChannel(9);

        if (autonomousEnableChannel > 1500) { // Full self-driving:
            // Do autonomous stuff:
            Serial.println("Autonomous navigation...");
            // Do something with autonomousSpeedChannel;
        } else if (cruiseControlEnable > 1500) {    // Just speed control only. Manual steering
            Serial.println("cruise navigation...");
        } else if (tractionControl > 1750) {
            Serial.println("traction control");
        } else if (tractionControl < 1250) {
            Serial.println("Something else ... breaking?");
        } else {
//            manualControlFunc(oldServoVal, oldMotorVal, servoVal, motorVal);
            Serial.println("manual control");
        }

        if (oldServoVal != servoVal) {
            setAngle_us(servoVal);
//            Serial.print("Servo: \t");
//            Serial.print(servoVal); // display new value in microseconds on PC
//            Serial.print("\t - pulse: \t");
//            Serial.print(getPulseSize());
//            Serial.print("\n");
            oldServoVal = servoVal;
        }

        if (oldMotorVal != motorVal) {
            motor.setSpeed((motorVal - 1500.0) * (127.0 / 500.0));
//            Serial.print("Motor: \t");
//            Serial.print(motorVal); // display new value in microseconds on PC
//            Serial.print("\t - pulse: \t");
//            Serial.print((motorVal - 1500.0) * (127.0 / 500.0));
//            Serial.print("\n");
            oldMotorVal = motorVal;
        }

        // */
        // Printing of status of the encoders
        if ((millis() - encoderPrintTime) > 100) {
            encoderPrintTime = millis();

            wheelEncoders.calculateSpeeds();
            float frontSpeed = wheelEncoders.getSpeed(RIGHT_ENCODER);
            float backSpeed = wheelEncoders.getSpeed(BACK_ENCODER);

            if (backSpeed > 0 && frontSpeed > 0) {
                Serial.println(frontSpeed / backSpeed, 6);
            }
        }


        i++;
    }
}

void manualControlFunc(int &oldServoVal, int &oldMotorVal, int servoVal, int motorVal) {// Manual actuation of the car.
    if (oldServoVal != servoVal) {
        setAngle_us(servoVal);
        Serial.print("Servo: \t");
        Serial.print(servoVal); // display new value in microseconds on PC
        Serial.print("\t - pulse: \t");
        Serial.print(getPulseSize());
        Serial.print("\n");
        oldServoVal = servoVal;
    }

    if (oldMotorVal != motorVal) {
        motor.setSpeed((motorVal - 1500.0) * (127.0 / 500.0));
        Serial.print("Motor: \t");
        Serial.print(motorVal); // display new value in microseconds on PC
        Serial.print("\t - pulse: \t");
        Serial.print((motorVal - 1500.0) * (127.0 / 500.0));
        Serial.print("\n");
        oldMotorVal = motorVal;
    }
}

void stopAll() {
    motor.setSpeed(0);
    setAngle_us(1500);
}

ISR(ULTRASONIC_SENSORS_INT_vect) {
    ultrasonicSensors.handleInterrupt();
}


// Controls for the Servo (only slightly janky control implementation)
volatile const uint8_t ROLLOVER_SCALER_COUNT = 156;
volatile uint8_t ServoCurrentScalerCount = 0;
ISR(TIMER2_OVF_vect) {
    ServoCurrentScalerCount++;
    if (ServoCurrentScalerCount > ROLLOVER_SCALER_COUNT) {
        PORTD |= _BV(PD5);
        ServoCurrentScalerCount = 0;
    } else if (ServoCurrentScalerCount > getPulseSize()) {
        PORTD &= ~_BV(PD5);
    }
}

volatile uint8_t oldInt = 0;
volatile uint8_t newBits = 0;
volatile uint8_t changedBits = 0;
/**
 * ISR is used for pulse detection and counting, and will return speed.
 */
ISR(PCINT1_vect) {
//    PORTB |= _BV(PB5);  // Used to time function with oscilloscope
    newBits = PINC;
    changedBits = oldInt ^ newBits;
    oldInt = newBits;

//    Serial.print(oldInt);
//    Serial.print("\t");
//    Serial.print(newBits);
//    Serial.print("\t");
//    Serial.println(changedBits);

    if (changedBits & _BV(0)) {
        wheelEncoders.processInterrupt(BACK_ENCODER);
    }
    if (changedBits & _BV(1)) {
        wheelEncoders.processInterrupt(BACK_ENCODER);
    }

//    if (changedBits & _BV(2)) {
//        wheelEncoders.processInterrupt(LEFT_ENCODER);
//    }
//    if (changedBits & _BV(3)) {
//        wheelEncoders.processInterrupt(LEFT_ENCODER);
//    }

    if (changedBits & _BV(4)) {
        wheelEncoders.processInterrupt(RIGHT_ENCODER);
    }
    if (changedBits & _BV(5)) {
        wheelEncoders.processInterrupt(RIGHT_ENCODER);
    }
//    PORTB &= ~_BV(PB5);  // Used to time function with oscilloscope

    // */
}