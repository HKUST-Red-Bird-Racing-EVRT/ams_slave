#include "RegistersADC.hpp"

/**
* @brief Extracts and reassembles the mathematical 5-bit ADC Gain factor.
* * @details This function isolates the scattered bit fields, shifts them into 
* alignment, and merges them.
* - Bits 3-2 from GAIN1 are shifted left by 1 to occupy target bits 4-3.
* - Bits 7-5 from GAIN2 are shifted right by 5 to occupy target bits 2-0.
* * @return uint8_t A unified 5-bit integer (range 0 to 31) representing the absolute ADC Gain.
*/

uint8_t ADC_getCombinedGain()    // microvolts
{
    uint8_t upper_bits = (ADCGAIN1.getRawData() & ADCGAIN1_MASK) << 1; 
    uint8_t lower_bits = (ADCGAIN2.getRawData() & ADCGAIN2_MASK) >> 5; 
    return (upper_bits | lower_bits);
}


int8_t ADC_getOffset()   // millivolts
{
    uint8_t offset_raw = ADCOFFSET.getRawData();
    if (offset_raw >= 0x80)
    {
        return static_cast<int8_t>(offset_raw) - 256;
    }
    else
    {
        return static_cast<int8_t>(offset_raw);
    }
}