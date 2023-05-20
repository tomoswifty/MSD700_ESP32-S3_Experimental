#pragma once

class IM920s
{
private:
    HardwareSerial &serialRef;
    const uint32_t baudRateArray[10] =
        {1200,
         2400,
         4800,
         9600,
         19200,
         38400,
         57600,
         115200,
         230400,
         460800};
    bool getCommandStatus();

public:
    IM920s(HardwareSerial &serialRef);
    void begin(uint32_t baudrate);
    void begin(uint32_t baudrate, int32_t config, uint8_t rxPin, uint8_t txPin);
    bool parameterReset();
    String receive(bool debug = false);
    String receivePayload(bool debug = false);
    bool sendBroadcast(String payload);
    bool sendUnicast(uint8_t target, String payload);
    bool changeBaudrate(uint8_t baudrate);
    bool enableCharacterMode();
    bool disableCharacterMode();
    bool setGroupNumber();
    String getGroupNumber();
    bool setNodeNumber(uint8_t nodeNumber);
    uint16_t getNodeNumber();
    bool setChannel(uint8_t channel);
    uint8_t getChannel();
};