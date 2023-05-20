#pragma once

class JetsonAutoControl
{
private:
    uint32_t lastParseTime = 0;
    uint32_t timeoutThreshold_ms = 500;
    float velocityInDirectionOfTravel = 0;
    float angularVelocity = 0;
    bool isTimeout = true;

public:
    void parse(String payload);
    float getVelocityInDirectionOfTravel();
    float getAngularVelocity();
    void setTimeoutThreshold(uint32_t timeout_ms);
};