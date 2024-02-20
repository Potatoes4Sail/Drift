//
// Created by Patrick on 2024-02-20.
//

#include "PWM.h"
void PWMTest(){
    DDRD |= _BV(DDD6); // Sets PD6 to output

    OCR0A = (uint8_t)1; //(50 * 2.55)
    // set PWM for 50% duty cycle

    TCCR0A |= (1 << COM0A1);
    // set none-inverting mode

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

    TCCR0B |= 0b010;
    // set prescaler to 8 and starts PWM

    while (1);
    {
        // we have a working Fast PWM
    }
}
