#include "ADCCalibration.hpp"
#include "AmsState.hpp"
#include "Registers.hpp"

uint16_t Bq76940AdcCalibration::ADCGain() const // microvolts
{
    return 365 + static_cast<uint16_t>(getCombinedGain());
}

int8_t Bq76940AdcCalibration::ADCOffset() const // millivolts
{
    uint8_t offset_raw = getSignedOffset();
    if (offset_raw >= 0x80)
    {
        return static_cast<int8_t>(offset_raw) - 256;
    }
    else
    {
        return static_cast<int8_t>(offset_raw);
    }
}

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

constexpr uint16_t ADCGAIN = Bq76940AdcCalibration::ADCGain() const;
constexpr int8_t ADCOFFSET = Bq76940AdcCalibration::ADCOffset() const;

constexpr uint16_t OV_TRIP_FULL = 