

#ifdef INCLUDECUSTOM
#define millis() countMillis()
#define micros() countMicros()
#else

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#endif

#include <avr/io.h>
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
    // Timer interrupt stuff
    TCCR1A = 0x00;          // Sets timer/counter1 control register back to default state.
    TCCR1B = 0b001;         // Sets /1024 prescaler
    TCNT1 = 0; // clear timer values
    TIMSK1 = 0;
    _BV(TOIE1);

    PCICR |= _BV(PCIE0);

    init();
    Serial.begin(115200);
    DDRB |= _BV(DDB5); // Set builtin LED to output


    sei();


    unsigned long startTime1 = 0;
    unsigned long startTime2 = 0;

    Serial.println(TCCR1B);

    while (true) {
        // Serial.println(millis());
        if ((millis() - startTime1) > 100) {

            ultrasonicSensors.sendEcho();
            startTime1 = millis();
        }

        /*int main() {
            init();
            Serial.begin(115200);
            if ((millis() - startTime2) > 250) {
//            ultrasonicSensors.sendEcho();
                ultrasonicSensors.readLeftDistance();
                ultrasonicSensors.readFrontDistance();
                ultrasonicSensors.readRightDistance();
//            Serial.println(ultrasonicSensors.readLeftDistance());

                return 1;
                startTime2 = millis();
            }
        }*/
    }
}

ISR(ULTRASONIC_SENSORS_INT_vect) {
    ultrasonicSensors.handleInterrupt();
}
