//
// Created by Patrick on 2024-02-20.
//
#ifdef DEBUG_BUILD

#include <HardwareSerial.h>

#endif

#include "helperFunctions.h"

static uint8_t oldInterruptState = SREG;

void stopInterrupts() {
    oldInterruptState = SREG;
    cli(); // Clears interrupt bank
    // return oldSREG;
}

void startInterrupts() {
    SREG = oldInterruptState;
}

void customInitialization() {
    // Initalize the various timers:

    stopInterrupts();

    // Timer1 (used for timing by ultrasonic sensors, pulse encoders, and ...?)
    TCCR1A = 0x00;          // Sets timer/counter1 control register back to default state.
    TCCR1B = 0b001;         // Sets /1024 prescaler
    TCNT1 = 0; // clear timer values
    TIMSK1 = 0;
    _BV(TOIE1);

    // Timer2:
    //      Used by motor and servo control (in very sketchy method ;c)
    TCCR2A = 0;
    TCCR2A |= (1 << COM2A1); // Enables PWM output for timer2 side A

    TCCR2A |= (1 << COM2B1); // Enables PWM output for timer2 side B
    TCCR2A |= (1 << WGM21) | (1 << WGM20); // Enables fast PWM Mode

    TCCR2B = 0b010; // Sets Prescaler to 8 (~8 kHz), will start PWM at 0 duty cycle.

    OCR2A = (uint8_t) 0; // Set PWM to 0%
    OCR2B = (uint8_t) 0; // Set PWM to 0%
    // Setup interrupts
    TIMSK2 = _BV(TOIE2); // Enable the overflow interrupt
    sei();


    PCICR |= _BV(PCIE0) | _BV(PCIE1);

    // Initalize pins:
    startInterrupts();
    // Empty for now ~

#ifdef DEBUG_BUILD
    Serial.println("Initalized");
#endif
}

unsigned long countPulse(volatile uint8_t *port, uint8_t bit, uint8_t stateMask, unsigned long maxloops) {
    unsigned long width = 0;
    // wait for any previous pulse to end
    while ((*port & bit) == stateMask)
        if (--maxloops == 0)
            return 0;

    // wait for the pulse to start
    while ((*port & bit) != stateMask)
        if (--maxloops == 0)
            return 0;

    // wait for the pulse to stop
    while ((*port & bit) == stateMask) {
        if (++width == maxloops)
            return 0;
    }
    return width;
}

/// measurePulse - Measure the duration of a pulse of a given state:
/// \param pin - Pin to measure state of
/// \param pinState - state (HIGH/LOW) to measure duration
/// \param timeout - default timeout (100 ms)
/// \return Duration of pulse or 0 if duration is 0.`
uint16_t measurePulse(uint8_t pin, uint8_t pinState, unsigned long timeout) {
// cache the port and bit of the pin in order to speed up the
// pulse width measuring loop and achieve finer resolution.  calling
// digitalRead() instead yields much coarser resolution.
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);

    uint8_t stateMask = (pinState ? bit : 0);

// convert the timeout from microseconds to a number of times through
// the initial loop; it takes approximately 16 clock cycles per iteration
    unsigned long maxloops = microsecondsToClockCycles(timeout) / 16.0;
    unsigned long width = countPulse(portInputRegister(port), bit, stateMask, maxloops);

// prevent clockCyclesToMicroseconds to return bogus values if countPulseASM timed out
    if (width)
        return clockCyclesToMicroseconds(width * 16 + 16);
    else
        return 0;
}

#ifdef INCLUDECUSTOM
volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

ISR(TIMER0_OVF_vect) {
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    unsigned long m = timer0_millis;
    unsigned char f = timer0_fract;

    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX) {
        f -= FRACT_MAX;
        m += 1;
    }

    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
}

unsigned long millis() {
    unsigned long m;
    uint8_t oldSREG = SREG;

    // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer0_millis)
    cli();
    m = timer0_millis;
    SREG = oldSREG;

    return m;
}

unsigned long micros() {
    unsigned long m;
    uint8_t oldSREG = SREG, t;

    cli();
    m = timer0_overflow_count;
    t = TCNT0;

    if ((TIFR0 & _BV(TOV0)) && (t < 255))
        m++;

    SREG = oldSREG;

    return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

void startupFunction(){

    /*
     *  SETUP TIMER0 to be used with countMillis() and countMicros() functions.
     */
    // Loads the default arduino timer interrupts and such.
    sei();

    // Set Waveform to Fast PWM
    TCCR0A |= _BV(WGM01) | _BV(WGM00);

    // Timer 0 prescale factor to 64
    TCCR0B |= _BV(CS00) | _BV(CS01);

    // Enable interrupt mask
    TIMSK0 |= TOIE0;

    // Setup timers for PWM
    TCCR1B = 0;
    // prescaler to 64
    bitSet(TCCR1B, CS11);
    bitSet(TCCR1B, CS10);
    bitSet(TCCR1A, WGM10);
    // put timer 1 in 8-bit phase correct pwm mode

    bitSet(TCCR1A, WGM10);
// timer 2
    bitSet(TCCR2B, CS22);
    bitSet(TCCR2A, WGM20);

    bitSet(ADCSRA, ADPS2);
    bitSet(ADCSRA, ADPS1);
    bitSet(ADCSRA, ADPS0);
    bitSet(ADCSRA, ADEN);

    UCSR0B = 0;

}
#endif