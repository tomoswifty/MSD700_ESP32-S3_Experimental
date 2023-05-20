#include <Arduino.h>
#include <Wire.h>
#include <TsubakiMotor.h>
// #include <PulseInEsp32.h>

TsubakiMotor::TsubakiMotor(uint8_t forwardPin, uint8_t reversePin, uint8_t rotationPulsePin, uint8_t dacScl, uint8_t dacSda, uint8_t dacAddress) : forwardPin(forwardPin),
                                                                                                                                                   reversePin(reversePin),
                                                                                                                                                   rotationPulsePin(rotationPulsePin),
                                                                                                                                                   dacScl(dacScl),
                                                                                                                                                   dacSda(dacSda),
                                                                                                                                                   dacAddress(dacAddress),
                                                                                                                                                   maxRevolutionSpeed(3000)
{
    pinMode(forwardPin, OUTPUT);
    pinMode(reversePin, OUTPUT);

    digitalWrite(forwardPin, LOW);
    digitalWrite(reversePin, LOW);
}

void TsubakiMotor::setMaxRevolutionSpeed(uint16_t maxRevolutionSpeed)
{
    this->maxRevolutionSpeed = constrain(maxRevolutionSpeed, ABSOLUTE_MIN_REVOLUTION_SPEED, ABSOLUTE_MAX_REVOLUTION_SPEED);
}

uint16_t TsubakiMotor::getMaxRevolutionSpeed()
{
    return maxRevolutionSpeed;
}

void TsubakiMotor::forward(uint16_t speed_rpm, bool waitRotationStop)
{
    digitalWrite(reversePin, LOW);
    if (!(motorStatus.runningState == TsubakiMotorStateEnum::Forward))
    {
        if (measureRevolutionSpeed() > 0 & waitRotationStop)
        {
            return;
        }
    }
    speed_rpm = constrain(speed_rpm, ABSOLUTE_MIN_REVOLUTION_SPEED, maxRevolutionSpeed);
    int16_t dacValue = map(speed_rpm, ABSOLUTE_MIN_REVOLUTION_SPEED, maxRevolutionSpeed, 0, 4095);
    // dacWrite(analogInPin, constrain(dacValue, 0, 255));
    Wire.beginTransmission(dacAddress);
    Wire.write(0b00000000 | ((0b111100000000 & dacValue) >> 8));
    Wire.write(dacValue & 0b000011111111);
    Wire.endTransmission(1);

    digitalWrite(forwardPin, HIGH);
    motorStatus.runningState = TsubakiMotorStateEnum::Forward;
}

void TsubakiMotor::reverse(uint16_t speed_rpm, bool waitRotationStop)
{
    digitalWrite(forwardPin, LOW);
    if (!(motorStatus.runningState == TsubakiMotorStateEnum::Reverse))
    {
        if (measureRevolutionSpeed() > 0 & waitRotationStop)
        {
            return;
        }
    }
    speed_rpm = constrain(speed_rpm, ABSOLUTE_MIN_REVOLUTION_SPEED, maxRevolutionSpeed);
    int16_t dacValue = map(speed_rpm, ABSOLUTE_MIN_REVOLUTION_SPEED, maxRevolutionSpeed, 0, 4095);

    Wire.beginTransmission(dacAddress);
    Wire.write(0b00000000 | ((0b111100000000 & dacValue) >> 8));
    Wire.write(dacValue & 0b000011111111);
    Wire.endTransmission(1);

    digitalWrite(reversePin, HIGH);
    motorStatus.runningState = TsubakiMotorStateEnum::Reverse;
}

void TsubakiMotor::stop()
{
    // dacWrite(analogInPin, 0);
    Wire.beginTransmission(dacAddress);
    Wire.write(0b00000000);
    Wire.write(0b00000000);
    Wire.endTransmission(1);

    digitalWrite(forwardPin, LOW);
    digitalWrite(reversePin, LOW);
    if (measureRevolutionSpeed() == 0)
    {
        motorStatus.runningState = TsubakiMotorStateEnum::Stop;
    }
}

void TsubakiMotor::control(int16_t speed_rpm, bool waitRotationStop)
{
    if (speed_rpm > 0)
    {
        forward(speed_rpm, waitRotationStop);
    }
    else if (speed_rpm < 0)
    {
        reverse(-speed_rpm, waitRotationStop);
    }
    else
    {
        stop();
    }
}

uint16_t TsubakiMotor::measureRevolutionSpeed()
{
    uint32_t pulsePeriod = 0;

    // pulsePeriod = 2 * pulseInEsp32.pulseIn(8e5);
    // Serial.println(pulsePeriod);
    if (pulsePeriod == 0)
    {
        motorStatus.revolutionSpeed = 0;
        return 0;
    }
    motorStatus.revolutionSpeed = static_cast<uint16_t>(4e6 / pulsePeriod);
    return motorStatus.revolutionSpeed;
}