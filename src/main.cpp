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
    Serial.println("Start IBus2PWM");

    customInitialization(); // Needed for initializing the timers.

    Serial.println("Start IBus2PWM");

    volatile uint64_t i = 0;

    int saveServoVal = 0;
    int saveMotorVal = 0;

    int servoVal;
    int motorVal;

    int autonomousSpeedChannel = 0, cruiseControlEnable = 0, autonomousEnableChannel = 0, channel7 = 0, tractionControl = 0, channel9 = 0;
    unsigned long encoderPrintTime = 0;
    bool manualControl = true;
    while (true) {
        _delay_ms(100);

        // ================================================
        //
        //          READING IBUS INPUTS FROM RC.
        //
        // ================================================
        motorVal = IBus.readChannel(1);                         // get latest value for servo channel 2 (right vertical)
        servoVal = IBus.readChannel(
                3);                         // get latest value for servo channel 4 (left horizontal)
        autonomousSpeedChannel = IBus.readChannel(4);           // VRA - Adjust auto speed value?
        cruiseControlEnable = IBus.readChannel(5);                         // VRB - Unused
        autonomousEnableChannel = IBus.readChannel(
                6);          // SWA - also disables all inputs - use for full autonomy
        channel7 = IBus.readChannel(7);
        tractionControl = IBus.readChannel(8);                  //
        channel9 = IBus.readChannel(9);

        if (autonomousEnableChannel > 1500) { // Full self driving:
            // Do autonomous stuff:
            Serial.println("Autonomous navigation...");
            // Do something with autonomousSpeedChannel;
        } else if (cruiseControlEnable > 1500) {    // Just speed control only. Manual steering
            Serial.println("cruise navigation...");
        } else {
            manualControlFunc(saveServoVal, saveMotorVal, servoVal, motorVal);
        }

        // Printing of status of the encoders
        if ((millis() - encoderPrintTime) > 500) {
            encoderPrintTime = millis();

            wheelEncoders.calculateSpeeds();
            wheelEncoders.printEncoderStatus(BACK_ENCODER);
            wheelEncoders.printEncoderStatus(LEFT_ENCODER);
            wheelEncoders.printEncoderStatus(RIGHT_ENCODER);
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

uint8_t oldInt = 0;
/**
 * ISR is used for pulse detection and counting, and will return speed.
 */
ISR(PCINT1_vect) {
//    PORTB |= _BV(PB5);  // Used to time function with oscilloscope
    uint8_t changedBits = oldInt ^ PORTC;

    if (changedBits & _BV(digitalPinToBitMask(BACK_ENCODER_A))) {
        wheelEncoders.processInterrupt(BACK_ENCODER);
    }
    if (changedBits & _BV(digitalPinToBitMask(BACK_ENCODER_B))) {
        wheelEncoders.processInterrupt(BACK_ENCODER);
    }

    if (changedBits & _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_A))) {
        wheelEncoders.processInterrupt(LEFT_ENCODER);
    }
    if (changedBits & _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_B))) {
        wheelEncoders.processInterrupt(LEFT_ENCODER);
    }

    if (changedBits & _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_A))) {
        wheelEncoders.processInterrupt(RIGHT_ENCODER);
    }
    if (changedBits & _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_B))) {
        wheelEncoders.processInterrupt(RIGHT_ENCODER);
    }
//    PORTB &= ~_BV(PB5);  // Used to time function with oscilloscope
}