#include <Arduino.h>
#include <_utils.h>
#include <IM920sUtils.h>
#include <IM920sDS4Client.h>

IM920sDS4Client::IM920sDS4Client() : timeoutThreshold_ms(1000), lastParseTime(0), isConnected(false), isTimeout(true)
{
    Ds4Data ds4Data = {{128, 128, 128, 128}, {0, 0}, 0, 0, 1};
    DeadZone deadZone = {{0, 0, 0, 0}};
}

void IM920sDS4Client::parse(const String &payloadFromIM920s)
{
    if (((payloadFromIM920s.length() == 24 & payloadFromIM920s.indexOf(',') > 0)))
    {
        for (uint8_t i = 0; i < 4; i++) // i = 0, 1, 2, 3
        {
            ds4Data.hatValueArray[i] = IM920sUtils::hexStringToUint8_t(IM920sUtils::parseHexString(payloadFromIM920s, i));
        }
        for (uint8_t i = 4; i < 6; i++) // i = 4, 5
        {
            ds4Data.analogButtonValueArray[i - 4] = IM920sUtils::hexStringToUint8_t(IM920sUtils::parseHexString(payloadFromIM920s, i));
        }
        // Serial.println((IM920sUtils::hexStringToUint8_t(IM920sUtils::parseHexString(payloadFromIM920s, 6)) << 4) | (IM920sUtils::hexStringToUint8_t(IM920sUtils::parseHexString(payloadFromIM920s, 7)) >> 4), BIN);
        ds4Data.buttonsValue = (IM920sUtils::hexStringToUint8_t(IM920sUtils::parseHexString(payloadFromIM920s, 6)) << 4) | (IM920sUtils::hexStringToUint8_t(IM920sUtils::parseHexString(payloadFromIM920s, 7)) >> 4);
        // Serial.println(ds4Data.buttonsValue, BIN);
        ds4Data.isConnected = IM920sUtils::hexStringToUint8_t(IM920sUtils::parseHexString(payloadFromIM920s, 7)) & 1;

        lastParseTime = millis();
        isTimeout = false;
    }
    isTimeout = checkTimeout();
}

bool IM920sDS4Client::checkTimeout()
{
    if (millis() - lastParseTime > timeoutThreshold_ms)
    {
        Serial.println("Timeout");
    }
    return (millis() - lastParseTime > timeoutThreshold_ms);
}

bool IM920sDS4Client::connected() { return isConnected; }

bool IM920sDS4Client::timeout() { return isTimeout; }

void IM920sDS4Client::setTimeoutThreshold(uint16_t timeout)
{
    timeoutThreshold_ms = timeout;
}

void IM920sDS4Client::setLeftHatDeadZone(uint8_t x, uint8_t y)
{
    deadZone.deadZoneArray[0] = x;
    deadZone.deadZoneArray[1] = y;
}

void IM920sDS4Client::setRightHatDeadZone(uint8_t x, uint8_t y)
{
    deadZone.deadZoneArray[2] = x;
    deadZone.deadZoneArray[3] = y;
}

float IM920sDS4Client::getAnalogHatValue(Ds4AnalogHatEnum analogHat)
{
    if (isTimeout)
    {
        return 128;
    }

    uint8_t analogHatIndex = static_cast<uint8_t>(analogHat);
    float analogHatValue = ds4Data.hatValueArray[analogHatIndex];

    uint8_t upperDeadZone = 128 + deadZone.deadZoneArray[analogHatIndex];
    uint8_t lowerDeadZone = 128 - deadZone.deadZoneArray[analogHatIndex];

    // analogHatValue = constrain(analogHatValue, deadZone.deadZoneArray[analogHatIndex], 255);
    // analogHatValue = map(ds4Data.hatValueArray[analogHatIndex], deadZone.deadZoneArray[analogHatIndex], 255, 0, 255);

    if ((lowerDeadZone <= analogHatValue & analogHatValue <= 128) | (128 <= analogHatValue & analogHatValue <= upperDeadZone))
    {
        return 128;
    }
    else if (analogHatValue <= lowerDeadZone)
    {
        analogHatValue = constrain(analogHatValue, 0, lowerDeadZone);
        analogHatValue = utils::mapf(analogHatValue, 0, lowerDeadZone, 0, 128);
    }
    else if (upperDeadZone <= analogHatValue)
    {
        analogHatValue = constrain(analogHatValue, upperDeadZone, 255);
        analogHatValue = utils::mapf(analogHatValue, upperDeadZone, 255, 128, 255);
    }
    return analogHatValue;
}

float IM920sDS4Client::getAnalogButtonValue(Ds4AnalogButtonEnum analogButton)
{
    if (isTimeout)
    {
        return 0;
    }
    return ds4Data.analogButtonValueArray[static_cast<uint8_t>(analogButton)];
}

bool IM920sDS4Client::getButtonPress(Ds4ButtonEnum button)
{
    if (isTimeout)
    {
        return 0;
    }
    return (ds4Data.buttonsValue >> (11 - static_cast<uint8_t>(button))) & 1;
}