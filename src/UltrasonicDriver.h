//
// Created by Patrick on 2024-02-16.
//


#ifndef DRIFT_ULTRASONICDRIVER_H
#define DRIFT_ULTRASONICDRIVER_H

class UltrasonicDriver {
public:
    /// UltrasonicDriver Object - Object which is used with the HC-SR04 sensor
    /// \param triggerPinIN - digital pin number of trigger pin
    /// \param echoPinIN - digital pin number of echo pin
    UltrasonicDriver(uint8_t triggerPinIN, uint8_t echoPinIN);
    UltrasonicDriver();
    ~UltrasonicDriver() = default;

    /// pollSensor - Sends a pulse from the sensor
    int8_t pollSensor();
    int32_t readDistance();
    volatile bool handleInterrupt();

private:
    bool initialized = false;
    bool echoDetected = false; // Rename echoDetected to better variable name? Is false if it succeeded?
    uint8_t triggerPin, echoPin;
    volatile uint16_t startTime, endTime;

public:
    bool isInitialized() const;
    uint8_t getTriggerPin() const;
    uint8_t getEchoPin() const;

private:
    void triggerUltrasound() const; // Actually sends pulse to UltrasonicDriver sensor,
};


#endif //DRIFT_ULTRASONICDRIVER_H
