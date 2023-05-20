#include <Arduino.h>

#include <driver/pcnt.h>
#include <soc/pcnt_struct.h>

#include <PulseInEsp32.h>

int PulseInEsp32::pcntUnitNumberGlobal = 0;

PulseInEsp32::PulseInEsp32(uint8_t pinNum) : pcntUnitNumber(pcntUnitNumberGlobal)
{

    pinMode(pinNum, INPUT_PULLDOWN);

    unit = static_cast<pcnt_unit_t>(pcntUnitNumber);

    pcnt_config_t pcnt_config;

    pcnt_config.pulse_gpio_num = pinNum;
    pcnt_config.ctrl_gpio_num = PCNT_PIN_NOT_USED;
    pcnt_config.lctrl_mode = PCNT_MODE_KEEP;
    pcnt_config.hctrl_mode = PCNT_MODE_KEEP;
    pcnt_config.channel = PCNT_CHANNEL_0;
    pcnt_config.unit = unit;
    pcnt_config.pos_mode = PCNT_COUNT_INC;
    pcnt_config.neg_mode = PCNT_COUNT_DIS;
    pcnt_config.counter_h_lim = 15;
    pcnt_config.counter_l_lim = 0;

    pcnt_unit_config(&pcnt_config);
    pcnt_set_filter_value(unit, 1000);
    pcnt_filter_enable(unit);
    // pcnt_event_enable(unit, PCNT_EVT_H_LIM);
    pcnt_counter_pause(unit);
    pcnt_counter_clear(unit);

    // pcnt_isr_service_install(0);
    // pcnt_isr_handler_add(unit, PulseInEsp32::revolutionInterrupt, &time);

    pcnt_counter_resume(unit);

    pcntUnitNumberGlobal++;
}

void PulseInEsp32::revolutionInterrupt(void *time)
{
    uint32_t *a = static_cast<uint32_t *>(time);
    a[0] = micros() - a[1];
    a[1] = micros();
}

uint32_t PulseInEsp32::getCounterValue()
{
    int16_t ret;
    pcnt_get_counter_value(unit, &ret);
    return ret;
}

uint32_t PulseInEsp32::pulseIn(uint32_t timeout_us)
{
    if (getCounterValue() >= 13)
    {
        time[0] = micros() - time[1];
        time[1] = micros();
    }
    // Serial.println(getCounterValue());
    if (micros() - time[1] > timeout_us)
    {
        return 0;
    }
    else
    {
        return time[0] / 30;
    }
}