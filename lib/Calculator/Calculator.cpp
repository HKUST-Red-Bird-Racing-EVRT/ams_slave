#include "Calculator.hpp"

constexpr Calculator::Calculator(AmsState &ams_state_)
    : ams_state(ams_state_)
{
}

void Calculator::setAdcGain(uint8_t &byte1, uint8_t &byte2)
{
    #define REGISTER_ADCGAIN1_MASK  0x0C
    #define REGISTER_ADCGAIN2_MASK  0xE0

    uint8_t upper_bits = (byte1 & REGISTER_ADCGAIN1_MASK) << 1;
    uint8_t lower_bits = (byte2 & REGISTER_ADCGAIN2_MASK) >> 5;
    ADC_GAIN = 365 + (upper_bits | lower_bits);
}

void Calculator::setAdcOffset(uint8_t &byte1)
{
    ADC_OFFSET = static_cast<int8_t>(byte1);
}

/*
void Calculator::setRegisterVoltageThreshold(uint16_t voltage_ot, uint16_t voltage_ut, uint16_t voltage_ov, uint16_t voltage_uv)
{
    VOLTAGE_OT = voltage_ot;
    VOLTAGE_UT = voltage_ut;
    VOLTAGE_OV = ((voltage_ov - ADC_OFFSET) * 1000) / ADC_GAIN;
    VOLTAGE_UV = ((voltage_uv - ADC_OFFSET) * 1000) / ADC_GAIN;
}
*/

uint16_t Calculator::getRealVoltage(uint16_t &data)
{
    return (data * ADC_GAIN / 1000 + ADC_OFFSET);
}

bool Calculator::isOverTemperature(uint16_t &data)
{
    return (data > VOLTAGE_OT);
}

bool Calculator::isUnderTemperature(uint16_t &data)
{
    return (data < VOLTAGE_UT);
}

bool Calculator::isOverVoltage(uint16_t &data)
{
    return (data > VOLTAGE_OV);
}

bool Calculator::isUnderVoltage(uint16_t &data)
{
    return (data < VOLTAGE_UV);
}

bool Calculator::isCellBalActivated(uint8_t &byte)
{
    #define REGISTER_SYS_CTRL2_DSG_ON 0x02

    return !(byte & REGISTER_SYS_CTRL2_DSG_ON);
}

void Calculator::setVoltageMin()
{
    uint16_t min_voltage = 0;

    for (uint8_t index = 0; index < NUM_VC; ++index)
    {
        if (ams_state.cell_voltages[index] > min_voltage)
        {
            min_voltage = ams_state.cell_voltages[index];
        }
    }

    VOLTAGE_MIN = min_voltage;
}

uint16_t Calculator::getCellBalFlags()
{
    uint16_t flags = 0;

    for (uint8_t index = 0; index < NUM_VC; ++index)
    {
        if (ams_state.cell_voltages[index] >= VOLTAGE_START && ams_state.cell_voltages[index] - VOLTAGE_MIN >= VOLTAGE_DELTA)
        {
            flags |= 1 << index;
        }
    }

    return flags;
}