//
// Created by Patrick on 2024-02-20.
//

#include "helperFunctions.h"

static uint8_t oldInterruptState = SREG;

void stopInterrupts(){
    oldInterruptState = SREG;
    cli(); // Clears interrupt bank
    // return oldSREG;
}

void startInterrupts(){
    SREG = oldInterruptState;
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
/// \return Duration of pulse or 0 if duration is 0.
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