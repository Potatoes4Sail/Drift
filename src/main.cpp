#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#include "Ultrasonic.h"
#include "helperFunctions.h"
#include "L298Driver.h"
#include "servoDriver.h"
#include "pinDefinition.h"
#include "testCases.h"

int main() {
    servoDriverInit();

    uint8_t num = 0;
    int wait = 1000;
    while (true) {
        setAngleA(0);
        _delay_ms(wait);
#if USE_SERVO_PIN_2
        setAngleB(90);
#endif
        _delay_ms(wait);
    }

    return 1;
}