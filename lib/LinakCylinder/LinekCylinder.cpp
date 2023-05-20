#include <LinakCylinder.h>

LinakCylinder::LinakCylinder(uint8_t forwardPin, uint8_t reversePin) : forwardPin(forwardPin), reversePin(reversePin)
{
    pinMode(forwardPin, OUTPUT);
    pinMode(reversePin, OUTPUT);

    digitalWrite(forwardPin, LOW);
    digitalWrite(reversePin, LOW);
}

void LinakCylinder::forward()
{
    digitalWrite(reversePin, LOW);
    digitalWrite(forwardPin, HIGH);
}

void LinakCylinder::reverse()
{
    digitalWrite(forwardPin, LOW);
    digitalWrite(reversePin, HIGH);
}

void LinakCylinder::stop()
{
    digitalWrite(forwardPin, LOW);
    digitalWrite(reversePin, LOW);
}