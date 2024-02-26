

#ifdef INCLUDECUSTOM
//#define millis() countMillis()
//#define micros() countMicros()
#else
#include <Arduino.h>
#endif

#include <avr/io.h>
#include "ultrasonic.h"
#include "pinDefinition.h"

// Global variables
Ultrasonic ultrasonicSensors = Ultrasonic(ULTRASONIC_SENSOR_TRIGGER_PIN,
                                          ULTRASONIC_SENSOR0_ECHO_PIN,
                                          ULTRASONIC_SENSOR1_ECHO_PIN,
                                          ULTRASONIC_SENSOR2_ECHO_PIN);

int main() {
    Serial.begin(115200);
    DDRB |= _BV(DDB5); // Set builtin LED to output

    // Timer interrupt stuff
    TCCR1A = 0x00;          // Sets timer/counter1 control register back to default state.
    TCCR1B = 0b100;         // Sets /1024 prescaler
    TCNT1 = 64754;          // Set overflow counter to 1 second
    TIMSK1 |= _BV(TOIE1);
    PCICR |= _BV(PCIE0);
    sei();

    unsigned long startTime = 0;

    while (true) {
        if ((millis() - startTime) > 1000) {
            ultrasonicSensors.sendEcho();
            startTime = millis();
        }

        Serial.println(ultrasonicSensors.readLeftDistance());
    }
}

ISR(ULTRASONIC_SENSORS_INT_vect) {
    ultrasonicSensors.handleInterrupt();
}
