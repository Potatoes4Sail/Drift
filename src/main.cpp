#include <avr/io.h>
#include <util/delay.h>
#include "Ultrasonic.h"


int main(void) {
//    Serial.begin(9600);

    Ultrasonic sensor1(DDRB, DDB1, DDRB, DDB2);
    sensor1.pollSensor();

//    DDRB |= _BV(DDB5); // Set LED as an output
//    DDRB |= _BV(DDB1); // Set trigger to output
//    DDRB &= ~_BV(DDB2); // Set echo to input
//    DDRB &= ~_BV(DDB3);
//
    while (0) {
        PORTB |= _BV(PORTB5);
        _delay_ms(100);
        PORTB &= ~_BV(PORTB5);
        _delay_ms(10);
        // triggerUltrasound();

        // // Serial.println("sent a pulse");

        while (true) {
            if(_BV(PINB2) & PINB){
// //                Serial.println("hi");
                for (int i = 0; i < 10; ++i) {
                    PORTB ^= _BV(PORTB5);
                    _delay_ms(50);
                }
                break;
            }
        }
    }

    return 1;
}