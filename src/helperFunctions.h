//
// Created by Patrick on 2024-02-20.
//

#ifndef DRIFT_HELPERFUNCTIONS_H
#define DRIFT_HELPERFUNCTIONS_H

#include "pinDefinition.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define SET_BIT(PORT, BIT) PORT |= _BV(BIT);
#define CLR_BIT(PORT, BIT) PORT &= ~_BV(BIT);
#define TGL_BIT(PORT, BIT) PORT ^= _BV(BIT);

/// Uses macros to include the default Arduino define statements, such as constant values, simple functions and port mapping
#ifndef Arduino_h
//#define Arduino_h

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define min(a, b) ((a)<(b)?(a):(b))
#define max(a, b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round_MACRO(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

// On the ATmega1280, the addresses of some of the port registers are
// greater than 255, so we can't store them in uint8_t's.
extern const uint16_t PROGMEM port_to_mode_PGM[];
extern const uint16_t PROGMEM port_to_input_PGM[];
extern const uint16_t PROGMEM port_to_output_PGM[];

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
// extern const uint8_t PROGMEM digital_pin_to_bit_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];
extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];

#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )

#endif

//TODO: Troubleshoot why this doesn't seem to be working properly.

#define digitalPinSetOutput(P) bitSet(*portModeRegister(digitalPinToPort(P)), digitalPinToBitMask(P))
#define digitalPinSetInput(P)  bitClear(*portModeRegister(digitalPinToPort(P)), digitalPinToBitMask(P))

#define digitalPinWrite(pin, value) bitWrite(*portOutputRegister(digitalPinToPort(pin)), digitalPinToBitMask(pin), value)
#define digitalPinToggle(pin) bitToggle(*portOutputRegister(digitalPinToPort(pin)), digitalPinToBitMask(pin))

// static uint8_t oldInterruptState = -1; // Compiler does not like this. @Ruben am I bring dumb?

void stopInterrupts();

void startInterrupts(); // uint8_t oldInterruptStatus);

unsigned long countPulse(volatile uint8_t *port, uint8_t bit, uint8_t stateMask, unsigned long maxloops);

uint16_t measurePulse(uint8_t pin, uint8_t pinState, unsigned long timeout = 100000L);


/*
 * TIMING FUNCTIONS USED BY ARDUINO. CAN REPLACE WITH CUSTOM ONES LATER.
 */
#ifdef INCLUDECUSTOM
unsigned long countMillis();
unsigned long countMicros();
void startupFunction();

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
#endif

#endif //DRIFT_HELPERFUNCTIONS_H
