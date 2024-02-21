#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#include "Ultrasonic.h"
#include "helperFunctions.h"
#include "L298Driver.h"
#include "pinDefinition.h"



int main() {
    Serial.begin(9600);

    /*
     * Ultrasonic sensors are currently not usable with motor drivers as they use the same pins (just need to change numbers)
     */
    Ultrasonic sensor1(9, 10);
    Ultrasonic sensor2(9, 11);
    Ultrasonic sensor3(9, 12);
    //*/

    L298Driver motor(MOTOR_PIN_PWM, MOTOR_PIN_FORWARD, MOTOR_PIN_REVERSE);
    motor.setSpeed(100);

    uint16_t dist1, dist2, dist3;

    while (true) {

        dist1 = (uint16_t) sensor1.pollSensor(); //TODO: Switch back to cm and use a uint8 to store this? Would give 2.5 m and allow for slightly faster polling
        dist2 = (uint16_t) sensor2.pollSensor();
        dist3 = (uint16_t) sensor3.pollSensor();

        Serial.print((String)"Sensor1: " + dist1 + "Sensor2: " + dist2 + "Sensor3: " + dist3 + "!\n");


    }
    return 1;
}