#include "RegistersPROTECT.hpp"

//  Register PROTECT3 Functions

uint8_t PROTECT3_getUVDelayCode()
{
    return (PROTECT3.getRawData() & 0xC0) >> 6;
}

uint8_t PROTECT3_getOVDelayCode()
{
    return (PROTECT3.getRawData() & 0x30) >> 4;
}