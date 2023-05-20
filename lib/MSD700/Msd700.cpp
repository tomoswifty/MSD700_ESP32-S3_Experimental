#include <Arduino.h>
#include <_utils.h>
#include <Msd700.h>
#include <TsubakiMotor.h>

Msd700::Msd700(
    uint8_t leftMotorForwardPin,
    uint8_t leftMotorReversePin,
    uint8_t leftMotorRotationPulsePin,
    uint8_t rightMotorForwardPin,
    uint8_t rightMotorReversePin,
    uint8_t rightMotorRotationPulsePin) : leftMotor(leftMotorForwardPin, leftMotorReversePin, leftMotorRotationPulsePin, 21, 47, 0x60),
                                          rightMotor(rightMotorForwardPin, rightMotorReversePin, rightMotorRotationPulsePin, 21, 47, 0x61)
{
}

float Msd700::calculateRevolutionSpeed(float circumferentialSpeed)
{
    float revolutionSpeed = static_cast<float>((circumferentialSpeed * 60) / (2 * PI * WHEEL_RADIUS) * MOTOR_REDUCTION_RATIO);

    return revolutionSpeed;
}

void Msd700::calculateInverseKinematics(float velocityInDirectionOfTravel, float angularVelocity) // 進行方向速度の単位は[m/s]，角速度の単位は[rad/s]
{
    float leftCircumferentialSpeed = 0;
    float rightCircumferentialSpeed = 0;

    velocityInDirectionOfTravel = constrain(velocityInDirectionOfTravel, -MAX_VELOCITY_IN_DIRECTION_OF_TRAVEL, MAX_VELOCITY_IN_DIRECTION_OF_TRAVEL);
    angularVelocity = constrain(angularVelocity, -MAX_ANGULAR_VELOCITY, MAX_ANGULAR_VELOCITY);

    if (velocityInDirectionOfTravel >= 0)
    {
        leftCircumferentialSpeed = velocityInDirectionOfTravel + DISTANCE_BETWEEN_WHEELS * angularVelocity;
        rightCircumferentialSpeed = velocityInDirectionOfTravel - DISTANCE_BETWEEN_WHEELS * angularVelocity;
    }
    else
    {
        leftCircumferentialSpeed = velocityInDirectionOfTravel + DISTANCE_BETWEEN_WHEELS * ((-1) * angularVelocity);
        rightCircumferentialSpeed = velocityInDirectionOfTravel - DISTANCE_BETWEEN_WHEELS * ((-1) * angularVelocity);
    }

    leftMotorRevolutionSpeed = static_cast<int16_t>(calculateRevolutionSpeed(leftCircumferentialSpeed));
    rightMotorRevolutionSpeed = static_cast<int16_t>(calculateRevolutionSpeed(rightCircumferentialSpeed));

    // Serial.print(leftMotorRevolutionSpeed);
    // Serial.print(',');
    // Serial.print(rightMotorRevolutionSpeed);
    // Serial.print('\n');
}

static float degreeToRadian(float degree)
{
    return static_cast<float>(degree / 180 * M_PI);
}

void Msd700::travelByInverseKinematics(float velocityInDirectionOfTravel, float angularVelocity)
{
    calculateInverseKinematics(velocityInDirectionOfTravel, angularVelocity);
    leftMotor.control(leftMotorRevolutionSpeed, false);
    rightMotor.control(rightMotorRevolutionSpeed, false);
}

void Msd700::travelByDirectControl(int16_t leftMotorSpeed, int16_t rightMotorSpeed)
{
    leftMotor.control(leftMotorSpeed, false);
    rightMotor.control(rightMotorSpeed, false);
}

int16_t Msd700::joystickToRevolutionSpeed(uint8_t input)
{
    if (input > 128)
    {
        input = constrain(input, 128, 255);
        return map(input, 128, 255, TsubakiMotor::ABSOLUTE_MIN_REVOLUTION_SPEED, leftMotor.getMaxRevolutionSpeed());
    }
    else if (input < 128)
    {
        input = constrain(input, 0, 128);
        return map(input, 0, 128, -leftMotor.getMaxRevolutionSpeed(), -TsubakiMotor::ABSOLUTE_MIN_REVOLUTION_SPEED);
    }
    else
    {
        return 0;
    }
}

float Msd700::joystickToVelocityInDirectionOfTravel(float input)
{
    if (input >= 128)
    {
        input = constrain(input, 128, 255);
        return utils::mapf(input, 128, 255, 0, MAX_VELOCITY_IN_DIRECTION_OF_TRAVEL);
    }
    else
    {
        input = constrain(input, 0, 128);
        return utils::mapf(input, 0, 128, -MAX_VELOCITY_IN_DIRECTION_OF_TRAVEL, 0);
    }
}

float Msd700::joystickToAngularVelocity(float input)
{
    if (input >= 128)
    {
        input = constrain(input, 128, 255);
        return utils::mapf(input, 128, 255, 0, MAX_ANGULAR_VELOCITY);
    }
    else
    {
        input = constrain(input, 0, 128);
        return utils::mapf(input, 0, 128, -MAX_ANGULAR_VELOCITY, 0);
    }
}

uint16_t Msd700::getLeftMotorRevolutionSpeed()
{
    return leftMotor.measureRevolutionSpeed();
}

uint16_t Msd700::getRightMotorRevolutionSpeed()
{
    return rightMotor.measureRevolutionSpeed();
}