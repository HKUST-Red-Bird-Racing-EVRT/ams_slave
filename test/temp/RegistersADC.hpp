#ifndef RegistersADC_HPP
#define RegistersADC_HPP

#include "RegistersClass.hpp"
#include <stdint.h>

inline Register ADCGAIN1 = Register(RegisterAddress::ADCGAIN1, 0x00);
inline Register ADCOFFSET = Register(RegisterAddress::ADCOFFSET, 0x00);
inline Register ADCGAIN2 = Register(RegisterAddress::ADCGAIN2, 0x00);

#define REGISTER_ADCGAIN1_MASK 0x0C
#define REGISTER_ADCGAIN2_MASK 0xE0

uint8_t ADC_getCombinedGain();

int8_t ADC_getOffset();

#endif // RegistersADC_HPP