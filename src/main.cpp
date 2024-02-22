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
    volatile uint8_t newVal = PINB & (_BV(ULTRASONIC_SENSOR0_INT) | _BV(ULTRASONIC_SENSOR1_INT) | _BV(ULTRASONIC_SENSOR2_INT));
    ultrasonicSensors.interruptTrigger(OLD_INT_BANK ^ newVal, micros());
    OLD_INT_BANK = newVal;
    PORTB ^= (1 << PORTB5);
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

//    ultrasonicSensors.pollNextSensor();
//    ultrasonicSensors.readSensor(1);

//    interruptTesting();

//    ultrasonicDriver sensor1(7, 8);
//    ultrasonicDriver sensor2(7, 9);
//    ultrasonicDriver sensor3(7, 10);
//
//    uint16_t dist1, dist2, dist3;
    Serial.println("Should never be here");
    while (true) {

//        dist1 = (uint16_t) sensor1.pollSensor(); //TODO: Switch back to cm and use a uint8 to store this? Would give 2.5 m and allow for slightly faster polling
//        _delay_ms(10);
//        dist2 = (uint16_t) sensor2.pollSensor();
//        _delay_ms(10);
//        dist3 = (uint16_t) sensor3.pollSensor();
//        _delay_ms(10);
//        Serial.print((String)"Sensor1: " + dist1 + "Sensor2: " + dist2 + "Sensor3: " + dist3 + "!\n");
//        digitalPinWrite(13, HIGH);

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

    TCCR1A = 0x00; // Sets timer/counter1 control register back to default state.
    TCCR1B = 0b100; // Sets /1024 prescaler
    TCNT1 = 64754; // Set overflow counter to 1 second

    // Page 112... idk

    TIMSK1 |= _BV(TOIE1);

    ULTRASONIC_SENSORS_PIN_MASK |= (_BV(ULTRASONIC_SENSOR0_INT) | _BV(ULTRASONIC_SENSOR1_INT) |
                                    _BV(ULTRASONIC_SENSOR2_INT));
    sei();


    unsigned long pastRanTime = 0;
    unsigned long pastRanTime2 = 0;
    uint16_t dist0, dist1, dist2;
    Serial.println("entering loop");
    while (true) {
        if ((millis() - pastRanTime) > 250) {
            ultrasonicSensors.pollNextSensor();
            pastRanTime = millis();
        }

        if ((millis() - pastRanTime2 )> 1000) {
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