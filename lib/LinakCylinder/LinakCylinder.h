#pragma once

#include <Arduino.h>

enum class LinakCylinderStateEnum
{
    Stop,
    Forward,
    Reverse
};

class LinakCylinder
{
private:
    LinakCylinderStateEnum runningState;

    uint8_t forwardPin;
    uint8_t reversePin;

public:
    LinakCylinder(uint8_t forwardPin, uint8_t reversePin);
    void reverse();
    void forward();
    void stop();
};