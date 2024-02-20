#include <avr/io.h>
#include <util/delay.h>
#include "Ultrasonic.h"
#include "helperFunctions.h"

#include <Arduino.h>

int main() {
    stopInterrupts();
    Ultrasonic sensor1(UltrasonicSensor1_triggerPinBank, DDB1, DDRB, DDB2);
    sensor1.pollSensor();
    startInterrupts();

    Ultrasonic s2(10, 11);
    s2.pollSensor();

    return 1;
}