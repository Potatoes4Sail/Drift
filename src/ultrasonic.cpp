//
// Created by Patrick on 2024-02-22.
//
#include "ultrasonic.h"

/// Constructor of Ultrasonic sensor.
/// \param trig - Pin shared between all triggers of the sensors
/// \param echoPins - Array of pins used for echo responses
/// \param n - number of sensors used
Ultrasonic::Ultrasonic(uint8_t trig, uint8_t echoPins[], uint8_t n) {
    // Initialize variables for object
    triggerPin = trig;
    this->n = n;
    currentPolledSensor = 0;

    this->sensorArray = new ultrasonicDriver[n];
    this->echoPins = new uint8_t[n];
    this->distances = new uint16_t[n];

    // Initialize each of the objects
    for (uint8_t i = 0; i < n; ++i) {
        this->echoPins[i] = echoPins[i];
        this->sensorArray[i] = ultrasonicDriver(this->triggerPin, this->echoPins[i]);
        distances[i] = 0;
    }

    // Turns on pin change interrupt control register
    PCICR |= _BV(PCIE0);

    // Enable interrupts:
    // Sets pin mask for all ultrasonic sensor echo pins
    ULTRASONIC_SENSORS_PIN_MASK |= ULTRASONIC_SENSOR_INTERRUPT_MASK;

    // Enable global interrupts:
    sei();

    timing = false;

}

/// Default destructor
Ultrasonic::~Ultrasonic() {
    // Delete all the dynamically allocated arrays
    delete[] sensorArray;
    delete[] echoPins;
    delete[] distances;
} // Destructor

/// pollNextSensor
/// Will call the following sensor. This function lacks a timeout (as of right now),
/// if called too frequently ~60 ms min it will not read any distances
void Ultrasonic::pollNextSensor() {
//    Serial.print((String) "Current sensor: " + currentPolledSensor + "\tstartTime = " + startTime + "\t endTime = " + endTime + "\t timing " + timing + "\n");
    if (!timing) {
        uint16_t deltaTime = endTime - startTime; // Calculate the difference in time
        distances[currentPolledSensor] = (deltaTime / 5.88235f);
    } else { // Failed to return a proper value
//        Serial.println((String) "Function was still timing when this happened;\t startTime = " + startTime + "\t endTime = " + endTime + "\n");
        startTime = 0;
        endTime = 0;
        timing = false;
//        distances[currentPolledSensor]--;
    }

    if (++currentPolledSensor >= n) currentPolledSensor = 0;
    sensorArray[currentPolledSensor].pollSensor();
}

///
/// \param sensorNumber - number (0 to n) of sensor to get distance
/// \return Returns distance (in mm) for given ultrasonic sensor
uint16_t Ultrasonic::readSensorData(uint8_t sensorNumber) const {
    //return sensorArray;
    if (sensorNumber < n) {
        return distances[sensorNumber];
    }
    return -1;
}

void volatile Ultrasonic::interruptTrigger(uint8_t pinMask, uint32_t time) {
//    Serial.print((String) "currentPolledSensor = " + currentPolledSensor);
//    Serial.print("\t 0 = ");
//    Serial.print((pinMask & _BV(ULTRASONIC_SENSOR0_INT)),BIN);
//    Serial.print("\t 1 = ");
//    Serial.print((pinMask & _BV(ULTRASONIC_SENSOR1_INT)),BIN);
//    Serial.print("\t 2 = ");
//    Serial.print((pinMask & _BV(ULTRASONIC_SENSOR2_INT)),BIN);
//    Serial.print("\t 0 = ");
//    Serial.print((pinMask & _BV(ULTRASONIC_SENSOR0_INT)) && (currentPolledSensor == 0),BIN);
//    Serial.print("\t 1 = ");
//    Serial.print((pinMask & _BV(ULTRASONIC_SENSOR1_INT)) && (currentPolledSensor == 1),BIN);
//    Serial.print("\t 2 = ");
//    Serial.print((pinMask & _BV(ULTRASONIC_SENSOR2_INT)) && (currentPolledSensor == 2),BIN);
//    Serial.println();


//    if (((pinMask & _BV(ULTRASONIC_SENSOR0_INT)) && (currentPolledSensor == 0)) ||
//        ((pinMask & _BV(ULTRASONIC_SENSOR1_INT)) && (currentPolledSensor == 1)) ||
//        ((pinMask & _BV(ULTRASONIC_SENSOR2_INT)) && (currentPolledSensor == 2))) {
//        Serial.print((String) "currentPolledSensor = " + currentPolledSensor + "\n");
    // compares the pin passed by the interrupt function with bit shifted via number of sensors
    if (pinMask & _BV(currentPolledSensor)) {
        if (!timing) {
            startTime = time;
            timing = true;
        } else {
            endTime = time;
            timing = false;
        }
    }
}