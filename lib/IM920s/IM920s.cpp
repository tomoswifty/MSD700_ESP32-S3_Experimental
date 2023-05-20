#include <Arduino.h>
#include <IM920s.h>
#include <IM920sUtils.h>

IM920s::IM920s(HardwareSerial &serialRef) : serialRef(serialRef){};

void IM920s::begin(uint32_t baudrate)
{
    serialRef.begin(baudrate);
}

void IM920s::begin(uint32_t baudrate, int32_t config, uint8_t rxPin, uint8_t txPin)
{
    serialRef.begin(baudrate, config, rxPin, txPin);
}

bool IM920s::parameterReset()
{
    serialRef.println("PCLR");
    return getCommandStatus();
}

String IM920s::receive(bool debug)
{
    String response = "";

    response = serialRef.readStringUntil('\n');
    if (debug)
    {
        Serial.println(response);
    }
    return response;
}

String IM920s::receivePayload(bool debug)
{
    String receivedString = receive();
    if (receivedString.lastIndexOf(':') > -1)
    {
        receivedString = receivedString.substring(receivedString.lastIndexOf(':') + 1);
        if (debug)
        {
            Serial.println(receivedString);
        }
        return receivedString;
    }
    else
    {
        return "";
    }
}

bool IM920s::getCommandStatus()
{
    String response = receive();
    if (response.equals("OK"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool IM920s::sendBroadcast(String payload)
{
    String commandString = ("TXDA ");

    commandString.concat(payload);
    serialRef.println(commandString);

    return getCommandStatus();
}

bool IM920s::sendUnicast(uint8_t target, String payload)
{
    String commandString = "TXDU ";

    commandString.concat(target);
    commandString.concat(" ");
    commandString.concat(payload);
    serialRef.println(commandString);

    return getCommandStatus();
}

bool IM920s::changeBaudrate(uint8_t baudrate = 7)
{
    String commandString = "SBRT ";

    commandString.concat(baudrate);
    serialRef.println(commandString);
    serialRef.flush();

    bool status = getCommandStatus();

    serialRef.end();
    serialRef.begin(baudRateArray[baudrate]);

    return status;
}

bool IM920s::enableCharacterMode()
{
    serialRef.println("ECIO");
    return getCommandStatus();
}

bool IM920s::disableCharacterMode()
{
    serialRef.println("DCIO");
    return getCommandStatus();
}

bool IM920s::setGroupNumber()
{
    serialRef.println("STGN");
    return getCommandStatus();
}

String IM920s::getGroupNumber()
{
    serialRef.println("RDGN");
    return receive();
}

bool IM920s::setNodeNumber(uint8_t nodeNumber)
{
    String commandString = "STNN ";

    commandString.concat(nodeNumber);
    serialRef.println(commandString);

    return getCommandStatus();
}

uint16_t IM920s::getNodeNumber()
{
    serialRef.println("RDNN");
    return IM920sUtils::hexStringToUint16_t(receive());
}

bool IM920s::setChannel(uint8_t channel)
{
    String commandString = "STCH ";

    commandString.concat(channel);
    serialRef.println(commandString);

    return getCommandStatus();
}

uint8_t IM920s::getChannel()
{
    serialRef.println("RDCH");
    return IM920sUtils::hexStringToUint8_t(receive());
}