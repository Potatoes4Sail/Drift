#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#include "Ultrasonic.h"
#include "helperFunctions.h"
#include "L298Driver.h"
#include "pinDefinition.h"


/*
int main(void)
{
    DDRD |= (1 << DDD3);
    DDRB |= (1 << DDB3);
    // PD6 is now an output

    TCCR2A |= _BV(COM2A1);
    TCCR2A |= _BV(COM2B1);

    TCCR2A |= _BV(WGM21) | _BV(WGM20);
//    TCCR2A |= _BV(WGM20);

    TCCR2B |= 0b001;
    OCR2A = 180;
    OCR2B = 10;

    while (1);
    {
        // we have a working Fast PWM
    }
    return 1;
} //*/

int main() {

    return 1;
}

void motorTestSetup(){
    int8_t speed = 0;
    L298Driver motor(MOTOR_PIN_PWM, MOTOR_PIN_FORWARD, MOTOR_PIN_REVERSE, 25);
    motor.setSpeed(127);
//    _delay_ms(100);
//    motor.setSpeed(100);
    _delay_ms(1000);
    motor.setSpeed(0);
    _delay_ms(1000);

    motor.setSpeed(127);
//    _delay_ms(100);
//    motor.setSpeed(100);
    _delay_ms(1000);
    motor.setBrake(100);
    _delay_ms(1000);

}