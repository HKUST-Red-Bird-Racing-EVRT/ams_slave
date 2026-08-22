#ifndef CALCULATOR_OLD_HPP
#define CALCULATOR_OLD_HPP

#include <stdint.h>
#include "BoardConfig.h"
#include "BqRegsiters.h"

namespace CalculatorOld
{
    constexpr uint16_t VOLTAGE_OT = 900; // millidegrees Celsius (millivolts)
    constexpr uint16_t VOLTAGE_UT = 100;    // millidegrees Celsius (millivolts)

    uint16_t VOLTAGE_OV = 4200; // millivolts
    uint16_t VOLTAGE_UV = 3500; // millivolts

    constexpr uint16_t VOLTAGE_BOOT = 1000; // millivolts (temperature)
    constexpr uint16_t VOLTAGE_PORA = 5000; // millivolts (voltage)
    constexpr uint16_t VOLTAGE_SHUT = 3600; // millivolts

    constexpr uint16_t TIME_BOOT = 3; // milliseconds

    uint16_t ADC_GAIN;
    int8_t ADC_OFFSET;

    uint8_t DELAY_OV;
    uint8_t DELAY_UV;

    void setAdcGain(uint8_t &byte1, uint8_t &byte2);
    void setVoltageThreshold();
    uint16_t getIndividualCellVoltage(uint8_t &byte1, uint8_t &byte2);
    bool isOverVoltage(uint16_t &data);
    bool isUnderVoltage(uint16_t &data);
    void setVoltageDelay(uint8_t &byte);
    bool isOverTemperature(uint16_t &data);
    bool isUnderTemperature(uint16_t &data);
    bool isCellBalActivated(uint8_t &byte);
}

#endif // CALCULATOR_OLD_HPP