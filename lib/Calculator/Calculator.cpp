#include "Calculator.hpp"

Calculator::Calculator(AmsState &ams_)
    : ams(ams_)
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

void Calculator::setDischargingState(uint8_t &byte)
{
    #define REGISTER_SYS_CTRL2_DSG_ON 0x02

    if (byte & REGISTER_SYS_CTRL2_DSG_ON)
    {
        ams.discharge_active = true;
    }
}

void Calculator::setVoltageMin()
{
    uint16_t min_voltage = 0;

    for (uint8_t index = 0; index < NUM_VC; ++index)
    {
        if (ams.cell_voltages[index] > min_voltage)
        {
            min_voltage = ams.cell_voltages[index];
        }
    }

    VOLTAGE_MIN = min_voltage;
}

void Calculator::setCellBalFlags()
{
    uint16_t flags = CELLBAL_STATE_BIT;

    for (uint8_t index = 0; index < NUM_VC; ++index)
    {
        if (ams.cell_voltages[index] >= VOLTAGE_BALANCE && ams.cell_voltages[index] >= VOLTAGE_START && ams.cell_voltages[index] - VOLTAGE_BALANCE >= VOLTAGE_DELTA)
        {
            flags = 1 << (index + 1);
        }
    }

    if (!ams.cellbal_odd)
    {
        flags &= CELLBAL_EVEN_MASK;
    }
    else
    {
        flags &= CELLBAL_ODD_MASK;
    }

    ams.cellbal_flags = flags;
}