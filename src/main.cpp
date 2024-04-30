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

int main() {
    init(); // Needed for arduino functionality
//    Serial.begin(115200);
    IBus.begin(Serial);   // first instance - Uses compA of Timer0,
    servoDriverInit();
    Serial.println("Start IBus2PWM");

    customInitialization(); // Needed for initializing the timers.

    DDRB |= _BV(PB5);
    volatile uint64_t i = 0;

    int saveServoVal = 0;
    int saveMotorVal = 0;

    int servoVal;
    int motorVal;

    Serial.println("Start IBus2PWM");
    unsigned long encoderPrintTime = 0;
    while (true) {
        _delay_ms(100);

        // ================================================
        //
        //          READING IBUS INPUTS FROM RC.
        //
        // ================================================
        servoVal = IBus.readChannel(3); // get latest value for servo channel 4 (left horizontal)
        motorVal = IBus.readChannel(1); // get latest value for servo channel 2 (right vertical)


        // Manual actuation of the car.
        if (saveServoVal != servoVal) {
            setAngle_us(servoVal);
            Serial.print("Servo: \t");
            Serial.print(servoVal); // display new value in microseconds on PC
            Serial.print("\t - pulse: \t");
            Serial.print(getPulseSize());
            Serial.print("\n");
            saveServoVal = servoVal;
        }

        if (saveMotorVal != motorVal) {
            motor.setSpeed((motorVal - 1500.0) * (127.0 / 500.0));
            Serial.print("Motor: \t");
            Serial.print(motorVal); // display new value in microseconds on PC
            Serial.print("\t - pulse: \t");
            Serial.print((motorVal - 1500.0) * (127.0 / 500.0));
            Serial.print("\n");
            saveMotorVal = motorVal;
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

void printEncoderStatus(int whichEncoder) {

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