#pragma once

#include <Arduino.h>

class IM920sUtils
{
public:
    static String parseHexString(String string, uint8_t parsePosition)
    {
        return string.substring(3 * parsePosition, 2 + 3 * parsePosition); // XX,XX,XX,...のような文字列を分割
    }
    static String uint8_tToHexString(uint8_t data)
    {
        char buf[3];
        sprintf(buf, "%02x", data);
        return String(buf);
    }
    static uint8_t hexStringToUint8_t(String data)
    {
        char buf[4];
        data.toCharArray(buf, 4);
        return static_cast<uint8_t>(strtol(buf, NULL, 16));
    }
    static uint16_t hexStringToUint16_t(String data)
    {
        char buf[6];
        data.toCharArray(buf, 6);
        return static_cast<uint16_t>(strtol(buf, NULL, 16));
    }
};