

#ifdef INCLUDECUSTOM
#define millis() countMillis()
#define micros() countMicros()

#define init() //init()
#else

#include <Arduino.h>

#endif

#include <avr/io.h>
#include <util/delay.h>
#include <HardwareSerial.h>
#include "ultrasonic.h"
#include "pinDefinition.h"
#include "Ultrasonic.h"
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
/*int main() {
    init(); // Needed for arduino functionality
    Serial.begin(115200);

    customInitialization();

    servoDriverInit();

    L298Driver motor(MOTOR_PIN_PWM, MOTOR_PIN_FORWARD, MOTOR_PIN_REVERSE, 0);

    *portModeRegister(digitalPinToPort(SERVO_CONTROL_PIN)) |= digitalPinToBitMask(SERVO_CONTROL_PIN);
    DDRD |= _BV(PD5);
    micros();
    int8_t speed = 0;
    while (true) {
        motor.setSpeed(speed++);
        setAngle(45);
        _delay_ms(10);
//        _delay_ms(1000);
//        motor.setSpeed(0);
//        _delay_ms(1000);
//
//        motor.setSpeed(speed);
//        _delay_ms(1000);
//        motor.setBrake(100);
//        _delay_ms(1000);
    }

    uint32_t startTime1 = 0;
    uint32_t startTime2 = 0;

    // Beginnnings of main loop.
    while (1) {
        if ((millis()) - startTime1 > 100) {
            ultrasonicSensors.sendEcho();
        }

        if ((millis() - startTime2) > 250) {
            ultrasonicSensors.readLeftDistance();
            ultrasonicSensors.readFrontDistance();
            ultrasonicSensors.readRightDistance();
            Serial.println(ultrasonicSensors.readLeftDistance());
            startTime2 = millis();

        }
    }
    return 1;
}*/

ISR(ULTRASONIC_SENSORS_INT_vect) {
    ultrasonicSensors.handleInterrupt();
}


// Controls for the Servo
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
ISR(PCINT1_vect) {
    PORTB |= _BV(PB5);
    uint8_t changedBits = oldInt ^ PORTC;
    // Capturing the time at the start of the interrupt makes sure all of the encoders
    // will get the 'right' time when the trigger, as ones later down would otherwise get delayed

    if (changedBits & _BV(digitalPinToBitMask(MOTOR_ENCODER_A))) {
        wheelEncoders.processInterrupt(BACK_ENCODER, A, PORTC & _BV(digitalPinToBitMask(MOTOR_ENCODER_A)));
    }
    if (changedBits & _BV(digitalPinToBitMask(MOTOR_ENCODER_B))) {
        wheelEncoders.processInterrupt(BACK_ENCODER, B, PORTC & _BV(digitalPinToBitMask(MOTOR_ENCODER_B)));
    }

    if (changedBits & _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_A))) {
        wheelEncoders.processInterrupt(LEFT_ENCODER, A, PORTC & _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_A)));
    }
    if (changedBits & _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_B))) {
        wheelEncoders.processInterrupt(LEFT_ENCODER, B, PORTC & _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_B)));
    }

    if (changedBits & _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_A))) {
        wheelEncoders.processInterrupt(RIGHT_ENCODER, A, PORTC & _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_A)));
    }
    if (changedBits & _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_B))) {
        wheelEncoders.processInterrupt(RIGHT_ENCODER, B, PORTC & _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_B)));
    }
    PORTB &= ~_BV(PB5);
}