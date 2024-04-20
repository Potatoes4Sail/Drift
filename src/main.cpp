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

// Global variables
Ultrasonic ultrasonicSensors = Ultrasonic(ULTRASONIC_SENSOR_TRIGGER_PIN,
                                          ULTRASONIC_SENSOR0_ECHO_PIN,
                                          ULTRASONIC_SENSOR1_ECHO_PIN,
                                          ULTRASONIC_SENSOR2_ECHO_PIN);
encoders wheelEncoders = encoders();

int main() {
    init(); // Needed for arduino functionality
    Serial.begin(115200);

    customInitialization(); // Needed for initializing the timers.

    DDRB |= _BV(PB5);
    volatile uint64_t i = 0;
    while (true) {
        _delay_ms(100);
//        PORTB |= _BV(PB5);
//        _delay_ms(100);
//        PORTB &= ~_BV(PB5);
//        Serial.print("-");
        i++;
    }
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
    } else if (ServoCurrentScalerCount > PULSE_SIZE) {
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