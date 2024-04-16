

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

// Global variables
Ultrasonic ultrasonicSensors = Ultrasonic(ULTRASONIC_SENSOR_TRIGGER_PIN,
                                          ULTRASONIC_SENSOR0_ECHO_PIN,
                                          ULTRASONIC_SENSOR1_ECHO_PIN,
                                          ULTRASONIC_SENSOR2_ECHO_PIN);

int main() {
    init(); // Needed for arduino functionality
    Serial.begin(115200);

    customInitialization();


    servoDriverInit();

    L298Driver motor(MOTOR_PIN_PWM, MOTOR_PIN_FORWARD, MOTOR_PIN_REVERSE, 0);

    *portModeRegister(digitalPinToPort(SERVO_CONTROL_PIN)) |= digitalPinToBitMask(SERVO_CONTROL_PIN);
    DDRD |= _BV(PD5);

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
}

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