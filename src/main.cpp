#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= _BV(DDB5); // Set LED as an output

    while (1) {
        PORTB |= _BV(PORTB5);
        _delay_ms(10);
        PORTB &= ~_BV(PORTB5);
        _delay_ms(100);
    }
    return 1;
}