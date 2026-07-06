#ifndef RegistersADC_HPP
#define RegistersADC_HPP

#include "RegistersClass.hpp"
#include <stdint.h>

extern Register ADCGAIN1 = Register(static_cast<uint8_t>(RegisterAddress::ADCGAIN1), 0x00);
extern Register ADCOFFSET = Register(static_cast<uint8_t>(RegisterAddress::ADCOFFSET), 0x00);
extern Register ADCGAIN2 = Register(static_cast<uint8_t>(RegisterAddress::ADCGAIN2), 0x00);

#define ADCGAIN1_MASK 0x0C
#define ADCGAIN2_MASK 0xE0

uint8_t ADC_getCombinedGain();

int8_t ADC_getOffset();

#endif // RegistersADC_HPP