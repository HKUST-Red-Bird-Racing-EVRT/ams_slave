#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <stdint.h>
#include "AmsState.hpp"

class Calculator
{
    public:
        uint16_t VOLTAGE_OT = 900;  //	Overtemperature Voltage (millivolts)
        uint16_t VOLTAGE_UT = 100;  //	Undertemperature Voltage (millivolts)
        uint16_t VOLTAGE_OV = 4200; //	OverVoltage Voltage (millivolts)
        uint16_t VOLTAGE_UV = 3500; //	UnderVoltage Voltage (millivolts)
        uint16_t VOLTAGE_MAX = 4250; //  Max Voltage (millivolts) Will be defined later
        uint16_t VOLTAGE_MIN = 0;       //  Min Voltage (millivolts) Will be defined later
        uint16_t VOLTAGE_BALANCE = 0;   //  Cell Balancing Voltage (millivolts) Will be defined later

        uint32_t TIME_CELLBAL = 30000; //  Cell Balancing Time (milliseconds)

        Calculator(AmsState &ams_);
        Calculator() = delete;
        void setAdcGain(uint8_t &byte1, uint8_t &byte2);
        void setAdcOffset(uint8_t &byte1);
        //  void setRegisterVoltageThreshold(uint16_t voltage_ot, uint16_t voltage_ut, uint16_t voltage_ov, uint16_t voltage_uv);
        uint16_t getRealVoltage(uint16_t &data);
        bool isOverTemperature(uint16_t &data);
        bool isUnderTemperature(uint16_t &data);
        bool isOverVoltage(uint16_t &data);
        bool isUnderVoltage(uint16_t &data);
        void setDischargingState(uint8_t &byte);
        void setVoltageMin();
        void setCellBalFlags();

    private:
        AmsState &ams;
        static constexpr uint16_t VOLTAGE_BOOT = 1000; // millivolts (temperature)
        static constexpr uint16_t VOLTAGE_PORA = 5000; // millivolts (voltage)
        static constexpr uint16_t VOLTAGE_SHUT = 3600; // millivolts
        static constexpr uint16_t VOLTAGE_START = 3800; //  Minimum Voltage for Cell Balancing
        static constexpr uint16_t VOLTAGE_DELTA = 30;

        static constexpr uint16_t TIME_BOOT = 3; // milliseconds

        uint16_t ADC_GAIN = 0;
        int8_t ADC_OFFSET = 0;


};

#endif  // CALCULATOR_HPP