

#ifdef INCLUDECUSTOM
#define millis() countMillis()
#define micros() countMicros()

#define init() //init()
#else

#include <Arduino.h>

#endif

#include <avr/io.h>
#include <util/delay.h>
#include <HardwareSerial.h>
#include "ultrasonic.h"
#include "pinDefinition.h"
#include "Ultrasonic.h"
#include "helperFunctions.h"
#include "L298Driver.h"
#include "servoDriver.h"
#include "pinDefinition.h"
#include "testCases.h"

// Global variables
Ultrasonic ultrasonicSensors = Ultrasonic(ULTRASONIC_SENSOR_TRIGGER_PIN,
                                          ULTRASONIC_SENSOR0_ECHO_PIN,
                                          ULTRASONIC_SENSOR1_ECHO_PIN,
                                          ULTRASONIC_SENSOR2_ECHO_PIN);

int main() {
    init(); // Needed for arduino functionality
    Serial.begin(115200);

    customInitialization();
    uint32_t startTime1 = 0;
    uint32_t startTime2 = 0;

    // Beginnnings of main loop.
    while (1) {
        if ((millis()) - startTime1 > 100) {
            ultrasonicSensors.sendEcho();
        }

        if ((millis() - startTime2) > 250) {
            ultrasonicSensors.readLeftDistance();
            ultrasonicSensors.readFrontDistance();
            ultrasonicSensors.readRightDistance();
            Serial.println(ultrasonicSensors.readLeftDistance());
            startTime2 = millis();
        }
    }
    return 1;
}

ISR(ULTRASONIC_SENSORS_INT_vect) {
    ultrasonicSensors.handleInterrupt();
}