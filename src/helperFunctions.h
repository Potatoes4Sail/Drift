//
// Created by Patrick on 2024-02-20.
//

#ifndef DRIFT_HELPERFUNCTIONS_H
#define DRIFT_HELPERFUNCTIONS_H
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// static uint8_t oldInterruptState = -1; // Compiler does not like this. @Ruben am I bring dumb?

void stopInterrupts();
void startInterrupts(); // uint8_t oldInterruptStatus);

#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )

#endif //DRIFT_HELPERFUNCTIONS_H
