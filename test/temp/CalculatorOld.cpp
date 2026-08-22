#include "CalculatorOld.hpp"

void CalculatorOld::setAdcGain(uint8_t &byte1, uint8_t &byte2)
{
    uint8_t upper_bits = (byte1 & REGISTER_ADCGAIN1_MASK) << 1;
    uint8_t lower_bits = (byte2 & REGISTER_ADCGAIN2_MASK) >> 5;
    ADC_GAIN = 365 + (upper_bits | lower_bits);
}

void CalculatorOld::setVoltageThreshold()
{
    VOLTAGE_OV = ((VOLTAGE_OV - ADC_OFFSET) * 1000) / ADC_GAIN;
    VOLTAGE_UV = ((VOLTAGE_UV - ADC_OFFSET) * 1000) / ADC_GAIN;
}

uint16_t CalculatorOld::getIndividualCellVoltage(uint8_t &byte1, uint8_t &byte2)
{
    return ((static_cast<uint16_t>(byte1) & 0x3F) << 8) | byte2;
}

bool CalculatorOld::isOverVoltage(uint16_t &data)
{
    return (data > VOLTAGE_OV);
}

bool CalculatorOld::isUnderVoltage(uint16_t &data)
{
    return (data < VOLTAGE_UV);
}

void CalculatorOld::setVoltageDelay(uint8_t &byte)
{
    DELAY_OV = (byte & 0x30) >> 4;
    DELAY_UV = (byte & 0xC0) >> 6;
}

bool CalculatorOld::isOverTemperature(uint16_t &data)
{
    return (data > VOLTAGE_OT);
}

bool CalculatorOld::isUnderTemperature(uint16_t &data)
{
    return (data < VOLTAGE_UT);
}

bool CalculatorOld::isCellBalActivated(uint8_t &byte)
{
    return !(byte & 0x02);
}
