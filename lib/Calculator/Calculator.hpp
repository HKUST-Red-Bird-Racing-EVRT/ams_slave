/**
 * @file Calculator.hpp
 * @brief Public interface for AMS measurement calculations and thresholds.
 */

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <stdint.h>
#include "AmsState.hpp"

/** @brief Converts monitor readings and evaluates AMS safety thresholds. */
class Calculator
{
    public:
        uint16_t VOLTAGE_OT = 900;  /**< Overtemperature threshold in millivolts. */
        uint16_t VOLTAGE_UT = 100;  /**< Undertemperature threshold in millivolts. */
        uint16_t VOLTAGE_OV = 4200; /**< Overvoltage threshold in millivolts. */
        uint16_t VOLTAGE_UV = 3500; /**< Undervoltage threshold in millivolts. */
        uint16_t VOLTAGE_MAX = 4250; /**< Maximum allowed cell voltage in millivolts. */
        uint16_t VOLTAGE_MIN = 0; /**< Minimum cell voltage in millivolts. */
        uint16_t VOLTAGE_BALANCE = 0; /**< Requested balancing voltage in millivolts. */

        uint32_t TIME_CELLBAL = 30000; /**< Cell-balancing duration in milliseconds. */

        /** @param ams_ State object used by the calculator. */
        Calculator(AmsState &ams_);
        /** @brief Prevents construction without an AMS state object. */
        Calculator() = delete;

        /** @brief Sets ADC gain from the BQ76940 calibration registers. */
        void setAdcGain(uint8_t &byte1, uint8_t &byte2);

        /** @brief Sets ADC offset from the BQ76940 calibration register. */
        void setAdcOffset(uint8_t &byte1);

        /** @brief Converts a raw ADC voltage reading to millivolts. */
        uint16_t getRealVoltage(uint16_t &data);

        /** @brief Tests a temperature reading against the overtemperature threshold. */
        bool isOverTemperature(uint16_t &data);

        /** @brief Tests a temperature reading against the undertemperature threshold. */
        bool isUnderTemperature(uint16_t &data);

        /** @brief Tests a cell voltage against the overvoltage threshold. */
        bool isOverVoltage(uint16_t &data);

        /** @brief Tests a cell voltage against the undervoltage threshold. */
        bool isUnderVoltage(uint16_t &data);

        /** @brief Updates the AMS discharge state from a system-control register. */
        void setDischargingState(uint8_t &byte);

        /** @brief Calculates the lowest cell voltage currently stored in the AMS state. */
        void setVoltageMin();

        /** @brief Calculates cell-balancing flags using the configured thresholds. */
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