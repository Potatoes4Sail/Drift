

#ifdef INCLUDECUSTOM
//#define millis() countMillis()
//#define micros() countMicros()
#else
#include <Arduino.h>
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "ultrasonic.h"
#include "pinDefinition.h"
#include "helperFunctions.h"

// Global variables
//Ultrasonic ultrasonicSensors = Ultrasonic(ULTRASONIC_SENSOR_TRIGGER_PIN,(uint8_t[]) {ULTRASONIC_SENSOR0_ECHO_PIN, ULTRASONIC_SENSOR1_ECHO_PIN,ULTRASONIC_SENSOR2_ECHO_PIN}, 3);
Ultrasonic ultrasonicSensors = Ultrasonic(ULTRASONIC_SENSOR_TRIGGER_PIN,
                                          (uint8_t[]) {ULTRASONIC_SENSOR0_ECHO_PIN, ULTRASONIC_SENSOR1_ECHO_PIN,
                                                       ULTRASONIC_SENSOR2_ECHO_PIN}, 1);
volatile uint8_t OLD_INT_BANK = 0b00000000;

// Interrupts
//ISR(TIMER1_OVF_vect);

ISR(ULTRASONIC_SENSORS_INT_vect);

void interruptTesting();

int main() {
    // Either use the arduino init function, or the custom made startup function;
    // This will set up the registers to enable the timer for millis & micros
#ifdef INCLUDECUSTOM
    startupFunction();
#else
    init();
#endif

//    Serial.begin(9600);
    interruptTesting(); // Function used for testing


//    Serial.println("Should never be here");
    // debug message if function used for testing interrupts failed to get called.
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

    while (true) {
        // Checking every 100 ms, and poll sensor. This can likely be reduce to 60-80 ms/poll.
        if ((millis() - pastRanTime) > 100) {
            ultrasonicSensors.pollNextSensor();
            pastRanTime = millis();
        }

        // Every half second output the distances to the console.
        if ((millis() - pastRanTime2) > 500) {
            pastRanTime2 = millis();
            dist0 = ultrasonicSensors.readSensorData(0);
            dist1 = ultrasonicSensors.readSensorData(1);
            dist2 = ultrasonicSensors.readSensorData(2);
//            Serial.print((String) "Sensor0: " + dist0 + "\tSensor1: " + dist1 + "\tSensor2: " + dist2 + "!\n");
        }
    }
}

volatile uint16_t time = 0;
volatile bool timerSet = false;

// 4.44 us to .188 us
ISR(ULTRASONIC_SENSORS_INT_vect) {
    PORTB ^= (1 << PORTB5); // Use to debug to validate that the interrupt is triggering correctly.
//
    if (!timerSet) {
        time = micros();
        timerSet = true;
    } else {
        time -= micros();
        timerSet = false;
    }

// TODO: Use toggling of this port to measure total duration.
    // Bitwise and between port output and mask of interrupt pins.
    // This is not strictly needed, in the future would need if to see which function to call.
//    newVal = PINB; // & ULTRASONIC_SENSOR_INTERRUPT_MASK;
    // Removal of 2nd part of statement changes time from 4.76 us to 4.66 us. (.1 us faster)
//    ultrasonicSensors.interruptTrigger(OLD_INT_BANK ^ PINB, micros());
//    OLD_INT_BANK = PINB;
    PORTB ^= (1 << PORTB5); // Use to debug to validate that the interrupt is triggering correctly.
}

//ISR(TIMER1_OVF_vect) {
//    //    PORTB ^= (1 << PORTB5);
//}


//*/