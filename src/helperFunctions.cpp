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