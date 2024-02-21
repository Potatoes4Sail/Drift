#include <avr/io.h>
#include <util/delay.h>
#include "Ultrasonic.h"
#include <Arduino.h>
#include "helperFunctions.h"
#include "PWM.h"
#include "L298Driver.h"


int main() {
//    PWMTest();
//    Serial.begin(9600);

    /*
     * Ultrasonic sensors are currently not usable with motor drivers as they use the same pins (just need to change numbers)
     */
    Ultrasonic sensor1(9, 10);
    Ultrasonic sensor2(9, 11);
    Ultrasonic sensor3(9, 12);
    //*/

    L298Driver leftSide(13, 12, 11, OCR0A);
    L298Driver rightSide(14, 15, 16, OCR0B);

    uint16_t dist1, dist2, dist3;

    DDRB |= _BV(DDB5); // Sets B5 to output

    while (true) {

        dist1 = (uint16_t) sensor1.pollSensor(); //TODO: Switch back to cm and use a uint8 to store this? Would give 2.5 m and allow for slightly faster polling
        dist2 = (uint16_t) sensor2.pollSensor();
        dist3 = (uint16_t) sensor3.pollSensor();

        // Serial.print((String)"Sensor1: " + dist1 + "Sensor2: " + dist2 + "Sensor3: " + dist3 + "!\n");

        _delay_ms(50);
        PORTB |= _BV(PORTB5);
        _delay_ms(50);
        PORTB &= ~_BV(PORTB5);

    }
    return 1;
}