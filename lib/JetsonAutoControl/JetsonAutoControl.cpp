#include <Arduino.h>
#include <iostream>
#include <string>
#include <JetsonAutoControl.h>

void JetsonAutoControl::parse(String payload)
{
    if (payload.startsWith("Jetson:"))
    {
        // payload = payload.substring(payload.indexOf(":") + 1);
        velocityInDirectionOfTravel = (payload.substring(7, payload.indexOf(","))).toFloat();
        angularVelocity = (payload.substring(payload.indexOf(",") + 1, payload.length() - 1)).toFloat();

        lastParseTime = millis();
    }
}

float JetsonAutoControl::getVelocityInDirectionOfTravel()
{
    if (millis() - lastParseTime > timeoutThreshold_ms)
    {
        return 0;
    }
    else
    {
        return velocityInDirectionOfTravel;
    }
}
float JetsonAutoControl::getAngularVelocity()
{
    if (millis() - lastParseTime > timeoutThreshold_ms)
    {
        return 0;
    }
    else
    {
        return angularVelocity;
    }
}
void JetsonAutoControl::setTimeoutThreshold(uint32_t timeout_ms)
{
    timeoutThreshold_ms = timeout_ms;
}