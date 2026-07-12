#ifndef REGISTERSTS_HPP
#define REGISTERSTS_HPP

#include "RegistersClass.hpp"
#include <stdint.h>

extern Register TS1_HI = Register(RegisterAddress::TS1_HI, 0x00);
extern Register TS1_LO = Register(RegisterAddress::TS1_LO, 0x00);
extern Register TS2_HI = Register(RegisterAddress::TS2_HI, 0x00);
extern Register TS2_LO = Register(RegisterAddress::TS2_LO, 0x00);
extern Register TS3_HI = Register(RegisterAddress::TS3_HI, 0x00);
extern Register TS3_LO = Register(RegisterAddress::TS3_LO, 0x00);

extern Register TSENSOR[6] = {
    TS1_HI, TS1_LO,
    TS2_HI, TS2_LO,
    TS3_HI, TS3_LO
};

uint16_t TSENSOR_getVoltage(uint8_t sensor_index);

#endif // REGISTERSTS_HPP