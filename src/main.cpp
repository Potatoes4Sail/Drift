#include <Arduino.h>

void setup() {
    // write your initialization code here
    pinMode(LED_BUILTIN, OUTPUT); // Uses built in variable to set LED
}

void loop() {
// write your code here
    static const int timeMs = 1200; //  Allow for easily verifying you can compile & upload code

    digitalWrite(LED_BUILTIN, HIGH);
    delay(timeMs);
    digitalWrite(LED_BUILTIN, LOW);
    delay(timeMs);
}