//
// Created by Patrick on 2024-04-29.
//

#include <HardwareSerial.h>
#include <Arduino.h>
#include "bst7960Driver.h"

#define KP 50.0f
#define KI 20.0f
#define KD 3.0f

BST7960Driver::BST7960Driver(uint8_t forwardPWMPin, uint8_t reversePWMPin, uint8_t deadbandVal) {

    // Sets variables of the class.
    this->forwardPWMPin = forwardPWMPin;
    this->reversePWMPin = reversePWMPin;
    this->direction = NONE;
    this->speed = 0;
    this->deadband = deadbandVal;

    // ========================================
    //
    //              Configuring PWM
    //
    // ========================================

    *portModeRegister(digitalPinToPort(forwardPWMPin)) |= digitalPinToBitMask(
            forwardPWMPin);      // Sets PWM pin to output
    *portModeRegister(digitalPinToPort(reversePWMPin)) |= digitalPinToBitMask(
            reversePWMPin);      // Sets PWM pin to output
}

int BST7960Driver::setSpeed(int8_t speedVar) {
    uint8_t actualSpeed = 2 * abs(speedVar);

    unsigned long now = micros();
    double dt = (now - lastTime) / 1000000.0;
    if (dt == 0) return 0;

    // MEASURE SPEED AND COMPUTE ERROR
    // measuredSpeed = getMeasuredSpeed();
    float measuredSpeed = 0;
    static float error = actualSpeed - measuredSpeed;
    // We should do the difference in expected angular velocity and measured in RPM

    // Calculate PID
    integral += error * dt;
    float derivative = error - lastError;
    float output = (KP * error) + (KI * integral) + (KD * derivative);

    lastError = error;
    lastTime = now;

    // Saturate
    if(output < 0) output = 0;
    else if(output > 255) output = 255;
    setRegisterSpeed((uint8_t)output);

    // OLD CODE
//    this->direction = ((0 < speedVar) ? FORWARD : REVERSE);
//    this->speed = 2 * abs(speedVar);
//
//    if (speed < deadband) {
//        this->direction = NONE;
//        this->speed = 0;
//    }
//    setRegisterSpeed(direction);
    return 0;
}
//    // TODO: Would ideally set both pins at the same time to avoid different states,
//    //  but this will impose significant restrictions on which pins can be used, and has a minor improvement
//
//    if (speed > deadband) { // Set throttle to positive
//        setRegisterSpeed(FORWARD);  // Set PWM duty cycle to speed variable * 2
///*        if (MOTOR_PWM_REGISTER == 0) {
//            // Motor was previously off, set to full speed to start
//            MOTOR_PWM_REGISTER = (uint8_t) 255;
//        } else {
//            MOTOR_PWM_REGISTER = (uint8_t) 2 * speed; // Set PWM duty cycle to speed variable * 2
//        }*/
//    } else if (speed < -deadband) { // Set throttle to negative
//        setRegisterSpeed(REVERSE);  // Set PWM duty cycle to speed variable * 2
//    } else { // Turn off throttle
//        setRegisterSpeed(NONE);
//        *portOutputRegister(digitalPinToPort(reversePWMPin)) &= ~digitalPinToBitMask(
//                reversePWMPin); // Set reverse pin to low
//        *portOutputRegister(digitalPinToPort(forwardPWMPin)) &= ~digitalPinToBitMask(
//                forwardPWMPin); // Set forward pin to low
//
//        MOTOR_PWM_REGISTER = (uint8_t) 0; // Set PWM duty cycle to speed variable * 2
//    }
//    return 0;
//}


// TODO: Allow for setting the alternative register to interrupt, and imporve resolution of servo.
void BST7960Driver::setRegisterSpeed(uint8_t motor) {
    Serial.print("function - speed = ");
    Serial.print(speed);
    Serial.print("\t and motor#\t");
    Serial.print(motor);
    Serial.print("\n");

    TCCR2A &= ~0b11110000; // Turns off both of the outputs.
    // Set both comparator timers to 0. 
    MOTOR_PWM_FORWARD_REGISTER = 0;
    MOTOR_PWM_REVERSE_REGISTER = 0;

    *portOutputRegister(digitalPinToPort(forwardPWMPin)) &= ~digitalPinToBitMask(
            forwardPWMPin); // Set forward pin to low
    *portOutputRegister(digitalPinToPort(reversePWMPin)) &= ~digitalPinToBitMask(
            reversePWMPin); // Set forward pin to low

    switch (motor) {
        case 0:
            break;
        case FORWARD:
            Serial.println("FORWARD!");
            OCR2A = this->speed;
            TCCR2A |= _BV(COM2A1);
            break;
        case REVERSE:
            OCR2B = this->speed;
            TCCR2A |= _BV(COM2B1);
            break;
    }
}