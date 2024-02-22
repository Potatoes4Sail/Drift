#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ultrasonic.h"
#include "helperFunctions.h"
#include "pinDefinition.h"

Ultrasonic ultrasonicSensors = Ultrasonic(ULTRASONIC_SENSOR_TRIGGER_PIN,
                                          (uint8_t[]) {ULTRASONIC_SENSOR0_ECHO_PIN, ULTRASONIC_SENSOR1_ECHO_PIN,
                                                       ULTRASONIC_SENSOR2_ECHO_PIN}, 3);
volatile uint8_t OLD_INT_BANK = 0b00000000;
ISR(TIMER1_OVF_vect);

void interruptTesting();

ISR(ULTRASONIC_SENSORS_INT_vect) {
    // TODO: Troubleshoot this function not working reliably
    volatile uint8_t newVal =
            PINB & (_BV(ULTRASONIC_SENSOR0_INT) | _BV(ULTRASONIC_SENSOR1_INT) | _BV(ULTRASONIC_SENSOR2_INT));
    ultrasonicSensors.interruptTrigger(OLD_INT_BANK ^ newVal, micros());
    OLD_INT_BANK = newVal;
    PORTB ^= (1 << PORTB5); // Use to debug to validate that the interrupt is triggering correctly
}

void POC_ultrasonic_interrupts() {
    // Sets pin mask for all ultrasonic sensor echo pins
    ULTRASONIC_SENSORS_PIN_MASK |= (_BV(ULTRASONIC_SENSOR0_INT) | _BV(ULTRASONIC_SENSOR1_INT) |
                                    _BV(ULTRASONIC_SENSOR2_INT));

    // Enable global interrupts:
    sei();
}

int main() {
#if INCLUDECUSTOM
    startupFunction();
#else
    init();
#endif

    POC_ultrasonic_interrupts();

    Serial.begin(9600);

    interruptTesting();

    Serial.println("Should never be here");
    while (true) {

        digitalPinWrite(13, HIGH);
        _delay_ms(100);
        digitalPinWrite(13, LOW);
        _delay_ms(100);

    }
    return 1;
}

//*
void interruptTesting() {

    DDRB |= _BV(DDB5); // Set builtin LED to output

    // Timer interrupt stuff
    TCCR1A = 0x00; // Sets timer/counter1 control register back to default state.
    TCCR1B = 0b100; // Sets /1024 prescaler
    TCNT1 = 64754; // Set overflow counter to 1 second
    TIMSK1 |= _BV(TOIE1);
    sei();
    // Page 112... idk

    unsigned long pastRanTime = 0;
    unsigned long pastRanTime2 = 0;
    uint16_t dist0, dist1, dist2;
    Serial.println("entering loop");
    while (true) {
        if ((millis() - pastRanTime) > 250) {
            ultrasonicSensors.pollNextSensor();
            pastRanTime = millis();
        }

        if ((millis() - pastRanTime2) > 1000) {
            pastRanTime2 = millis();
            dist0 = ultrasonicSensors.readSensor(0);
            dist1 = ultrasonicSensors.readSensor(1);
            dist2 = ultrasonicSensors.readSensor(2);
            Serial.print((String) "Sensor0: " + dist0 + "\tSensor1: " + dist1 + "\tSensor2: " + dist2 + "!\n");
//            Serial.print((String) "Sensor start: " + ultrasonicSensors.startTime + "\t Sensor end: " + ultrasonicSensors.endTime + "\t timing: " + ultrasonicSensors.timing + "\n");
//            Serial.println(PORTB, BIN);
        }
    }
}

ISR(TIMER1_OVF_vect) {
//    _delay_ms(100);
//    PORTB ^= (1 << PORTB5);
}


//*/