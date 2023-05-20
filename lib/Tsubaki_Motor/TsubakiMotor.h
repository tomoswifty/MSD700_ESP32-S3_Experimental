#pragma once

// #include <PulseInEsp32.h>

enum class TsubakiMotorStateEnum
{
    Stop,
    Forward,
    Reverse
};

class TsubakiMotor
{
private:
    struct MotorStatus
    {
        TsubakiMotorStateEnum runningState;
        uint16_t revolutionSpeed;
    };

    uint16_t maxRevolutionSpeed;

    uint8_t forwardPin;
    uint8_t reversePin;
    uint8_t analogInPin;
    uint8_t rotationPulsePin;
    uint8_t dacSda;
    uint8_t dacScl;
    uint8_t dacAddress;

    // PulseInEsp32 pulseInEsp32;

    MotorStatus motorStatus = {TsubakiMotorStateEnum::Stop, 0};

public:
    static constexpr uint16_t ABSOLUTE_MAX_REVOLUTION_SPEED = 2500;
    static constexpr uint16_t ABSOLUTE_MIN_REVOLUTION_SPEED = 100;

    TsubakiMotor(uint8_t forwardPin, uint8_t reversePin, uint8_t rotationPulsePin, uint8_t dacScl, uint8_t dacSca, uint8_t dacAddress);
    void setMaxRevolutionSpeed(uint16_t maxRevolutionSpeed);
    uint16_t getMaxRevolutionSpeed();
    void forward(uint16_t speed_rpm, bool waitRotationStop = true);
    void reverse(uint16_t speed_rpm, bool waitRotationStop = true);
    void stop();
    void control(int16_t speed_rpm, bool waitRotationStop = true);
    uint16_t measureRevolutionSpeed();
};