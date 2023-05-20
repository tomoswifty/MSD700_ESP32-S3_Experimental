#pragma once
#include <TsubakiMotor.h>

class PinConfig
{
public:
    uint8_t leftMotorForwardPin;
    uint8_t leftMotorReversePin;
    uint8_t leftMotorRotationPulsePin;
    uint8_t rightMotorForwardPin;
    uint8_t rightMotorReversePin;
    uint8_t rightMotorRotationPulsePin;
};

class Msd700
{
private:
    // PinConfig &pinConfig;

    TsubakiMotor leftMotor;
    TsubakiMotor rightMotor;

    int16_t leftMotorRevolutionSpeed;
    int16_t rightMotorRevolutionSpeed;

    static float calculateRevolutionSpeed(float circumferentialSpeed);
    void calculateInverseKinematics(float velocityInDirectionOfTravel, float angularVelocity);
    static float degreeToRadian(float degree);

public:
    static constexpr float MAX_VELOCITY_IN_DIRECTION_OF_TRAVEL = 0.5; // 最大速度[m/s]
    static constexpr float MAX_ANGULAR_VELOCITY = 0.5;                // 最大角速度[rad/s]

    static constexpr float MOTOR_REDUCTION_RATIO = 50.0;    // モーターの減速比
    static constexpr float WHEEL_RADIUS = 0.135;            // 車輪半径[m]
    static constexpr float DISTANCE_BETWEEN_WHEELS = 0.600; // 車輪間の距離[m]

    Msd700(
        uint8_t leftMotorForwardPin,
        uint8_t leftMotorReversePin,
        uint8_t leftMotorRotationPulsePin,
        uint8_t rightMotorForwardPin,
        uint8_t rightMotorReversePin,
        uint8_t rightMotorRotationPulsePin);
    void travelByInverseKinematics(float velocityInDirectionOfTravel, float angularVelocity);
    void travelByDirectControl(int16_t leftMotorSpeed, int16_t rightMotorSpeed);

    int16_t joystickToRevolutionSpeed(uint8_t input);
    static float joystickToVelocityInDirectionOfTravel(float input);
    static float joystickToAngularVelocity(float input);

    uint16_t getLeftMotorRevolutionSpeed();
    uint16_t getRightMotorRevolutionSpeed();
};