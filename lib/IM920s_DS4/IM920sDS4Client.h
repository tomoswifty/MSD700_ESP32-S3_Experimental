#pragma once

enum class Ds4AnalogHatEnum
{
    LeftX,
    LeftY,
    RightX,
    RightY,
};

enum class Ds4AnalogButtonEnum
{
    L2,
    R2
};

enum class Ds4ButtonEnum
{
    Circle,
    Cross,
    Square,
    Triangle,
    Up,
    Down,
    Left,
    Right,
    L1,
    R1,
    L3,
    R3
};

class IM920sDS4Client
{
private:
    struct Ds4Data
    {
        uint8_t hatValueArray[4];          // 左X,左Y,右X,右Yの順
        uint8_t analogButtonValueArray[2]; // L2,R2の順
        uint16_t buttonsValue;             // 最上位ビットから4ビットは0．その後は順にCIRCLE,CROSS,SQUARE,TRIANGLE,UP,DOWN,LEFT,RIGHT,L1,R1,L3,R3
        bool isConnected;
        bool isTimeout;
    };

    struct DeadZone
    {
        uint8_t deadZoneArray[4]; // 左X,左Y,右X,右Yの順
    };

    bool isConnected;
    bool isTimeout;
    uint32_t lastParseTime;
    uint16_t timeoutThreshold_ms;
    Ds4Data ds4Data;
    DeadZone deadZone;
    bool checkTimeout();

public:
    IM920sDS4Client();
    void parse(const String &payloadFromIM920s);
    bool connected();
    bool timeout();
    void setTimeoutThreshold(uint16_t timeout);
    void setLeftHatDeadZone(uint8_t x, uint8_t y);
    void setRightHatDeadZone(uint8_t x, uint8_t y);
    float getAnalogHatValue(Ds4AnalogHatEnum analogHat);
    float getAnalogButtonValue(Ds4AnalogButtonEnum analogButton);
    bool getButtonPress(Ds4ButtonEnum button);
};