#include <avr/io.h>
#include <util/delay.h>
#include "Ultrasonic.h"
#include <Arduino.h>
#include "helperFunctions.h"



int main() {

    Serial.begin(9600);
    Ultrasonic sensor1(7, 8);
    Ultrasonic sensor2(7, 9);
    Ultrasonic sensor3(7, 10);

    uint16_t dist1, dist2, dist3;

    DDRB |= _BV(DDB5); // Sets B5 to output

    while (true) {

        dist1 = (uint16_t) sensor1.pollSensor(); //TODO: Switch back to cm and use a uint8 to store this? Would give 2.5 m and allow for slightly faster polling
        _delay_ms(50);
        dist2 = (uint16_t) sensor2.pollSensor();
        _delay_ms(50);
        dist3 = (uint16_t) sensor3.pollSensor();
        _delay_ms(50);
        Serial.print((String)"Sensor1: " + dist1 + "  \tSensor2: " + dist2 + "  \tSensor3: " + dist3 + "!\n");

        _delay_ms(50);
        PORTB |= _BV(PORTB5);
        _delay_ms(50);
        PORTB &= ~_BV(PORTB5);

    }
    return 1;
}