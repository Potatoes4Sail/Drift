#include <avr/io.h>
#include <util/delay.h>
#include "Ultrasonic.h"
#include "helperFunctions.h"

#include <Arduino.h>

int main() {

    Serial.begin(9600);
    Ultrasonic sensor1(9, 10);
    // Ultrasonic sensor1(DDRB, DDB1, DDRB, DDB2);

    DDRB |= _BV(DDB5); // Sets B5 to output
    while (true) {
        Serial.println(sensor1.pollSensor());

        sensor1.pollSensor();
        _delay_ms(50);
        PORTB |= _BV(PORTB5);
        _delay_ms(50);
        PORTB &= ~_BV(PORTB5);

    }

    Ultrasonic s2(10, 11);
    s2.pollSensor();

    return 1;
}