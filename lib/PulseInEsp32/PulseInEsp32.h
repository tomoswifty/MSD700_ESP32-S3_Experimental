#pragma once

#include <driver/pcnt.h>
#include <soc/pcnt_struct.h>

class PulseInEsp32
{
private:
    static int pcntUnitNumberGlobal;
    uint8_t pcntUnitNumber;

    uint32_t time[2] = {0, 0}; // time[0]最後に15パルス観測した時間，time[1]最後に15パルス観測した時間との差
    pcnt_unit_t unit;

public:
    PulseInEsp32(uint8_t pinNum);
    void init(uint8_t pinNum);
    static void revolutionInterrupt(void *time);
    uint32_t pulseIn(uint32_t timeout_us);
    uint32_t getCounterValue();
};