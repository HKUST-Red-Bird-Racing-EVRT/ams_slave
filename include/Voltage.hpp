#ifndef VOLTAGE_HPP
#define VOLTAGE_HPP

#include "AmsState.hpp"
#include "RegistersADC.hpp"
#include "RegistersVC.hpp"
#include "RegistersSYS.hpp"
#include "FaultFlags.hpp"

/*
To calculate the correct OV_TRIP and UV_TRIP register values for a device, use the following procedure:
1. Determine desired OV.
2. Read out [ADCGAIN] and [ADCOFFSET] from their corresponding registers. Note that ADCGAIN is stored in
units of µV/LSB, while ADCOFFSET is stored in mV.
3. Calculate the full 14-bit ADC value needed to meet the desired OV and UV trip thresholds as follows:
a. OV_TRIP_FULL = (OV – ADCOFFSET) ÷ ADCGAIN
b. UV_TRIP_FULL = (UV – ADCOFFSET) ÷ ADCGAIN
4. Remove the upper 2 MSB and lower 4 LSB from the full 14-bit value, retaining only the remaining middle 8
bits. This can be done by shifting the OV_TRIP_FULL and UV_TRIP_FULL binary values 4 bits to the right
and removing the upper 2 MSB.
5. Write OV_TRIP and UV_TRIP to their corresponding registers.
*/

constexpr uint16_t VOLTAGE_OV = 4200; // millivolts
constexpr uint16_t VOLTAGE_UV = 3500; // millivolts

constexpr uint16_t VOLTAGE_BOOT = 1000; // millivolts
constexpr uint16_t VOLTAGE_SHUT = 3600; // millivolts

constexpr uint16_t TIME_BOOT = 3; // milliseconds

const uint16_t ADC_GAIN = 365 + static_cast<uint16_t>(ADC_getCombinedGain());
const int8_t ADC_OFFSET = ADC_getOffset();

const uint16_t OV_TRIP_THRESHOLD_FULL = ((VOLTAGE_OV - ADC_OFFSET) * 1000) / ADC_GAIN;
const uint16_t UV_TRIP_THRESHOLD_FULL = ((VOLTAGE_UV - ADC_OFFSET) * 1000) / ADC_GAIN;

const uint8_t OV_TRIP_THRESHOLD = static_cast<uint8_t>((OV_TRIP_THRESHOLD_FULL >> 4) & 0x3F);
const uint8_t UV_TRIP_THRESHOLD = static_cast<uint8_t>((UV_TRIP_THRESHOLD_FULL >> 4) & 0x3F);

const uint16_t VBAT_THRESHOLD = 4 * ADC_GAIN * BAT_getVoltage() / 1000 + NUM_VC * ADC_OFFSET; // millivolts

void checkAndSetVoltageFaults(AmsState &state);

#endif // VOLTAGEHPP