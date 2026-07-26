#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <stdint.h>
#include "AmsState.hpp"

class Calculator
{
    public:
        uint16_t VOLTAGE_OT = 0;
        uint16_t VOLTAGE_UT = 0;
        uint16_t VOLTAGE_OV = 0;
        uint16_t VOLTAGE_UV = 0;
        constexpr Calculator(AmsState &ams_state_);
        Calculator() = delete;
        void setAdcGain(uint8_t &byte1, uint8_t &byte2);
        void setAdcOffset(uint8_t &byte1);
        void setVoltageThreshold(uint16_t voltage_ot, uint16_t voltage_ut, uint16_t voltage_ov, uint16_t voltage_uv);
        bool isOverTemperature(uint16_t &data);
        bool isUnderTemperature(uint16_t &data);
        bool isOverVoltage(uint16_t &data);
        bool isUnderVoltage(uint16_t &data);
        bool isCellBalActivated(uint8_t &byte);

    private:
        AmsState &ams_state;
        static constexpr uint16_t VOLTAGE_BOOT = 1000; // millivolts (temperature)
        static constexpr uint16_t VOLTAGE_PORA = 5000; // millivolts (voltage)
        static constexpr uint16_t VOLTAGE_SHUT = 3600; // millivolts

        static constexpr uint16_t TIME_BOOT = 3; // milliseconds

        uint16_t ADC_GAIN = 0;
        int8_t ADC_OFFSET = 0;


};

#endif  // CALCULATOR_HPP