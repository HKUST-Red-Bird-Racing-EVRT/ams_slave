#ifndef REGISTERSVC_HPP
#define REGISTERSVC_HPP

#include "RegistersClass.hpp"
#include <stdint.h>

extern Register VC1_HI = Register(RegisterAddress::VC1_HI, 0x00);
extern Register VC1_LO = Register(RegisterAddress::VC1_LO, 0x00);
extern Register VC2_HI = Register(RegisterAddress::VC2_HI, 0x00);
extern Register VC2_LO = Register(RegisterAddress::VC2_LO, 0x00);
extern Register VC3_HI = Register(RegisterAddress::VC3_HI, 0x00);
extern Register VC3_LO = Register(RegisterAddress::VC3_LO, 0x00);
extern Register VC4_HI = Register(RegisterAddress::VC4_HI, 0x00);
extern Register VC4_LO = Register(RegisterAddress::VC4_LO, 0x00);
extern Register VC5_HI = Register(RegisterAddress::VC5_HI, 0x00);
extern Register VC5_LO = Register(RegisterAddress::VC5_LO, 0x00);
extern Register VC6_HI = Register(RegisterAddress::VC6_HI, 0x00);
extern Register VC6_LO = Register(RegisterAddress::VC6_LO, 0x00);
extern Register VC7_HI = Register(RegisterAddress::VC7_HI, 0x00);
extern Register VC7_LO = Register(RegisterAddress::VC7_LO, 0x00);
extern Register VC8_HI = Register(RegisterAddress::VC8_HI, 0x00);
extern Register VC8_LO = Register(RegisterAddress::VC8_LO, 0x00);
extern Register VC9_HI = Register(RegisterAddress::VC9_HI, 0x00);
extern Register VC9_LO = Register(RegisterAddress::VC9_LO, 0x00);
extern Register VC10_HI = Register(RegisterAddress::VC10_HI, 0x00);
extern Register VC10_LO = Register(RegisterAddress::VC10_LO, 0x00);
extern Register VC11_HI = Register(RegisterAddress::VC11_HI, 0x00);
extern Register VC11_LO = Register(RegisterAddress::VC11_LO, 0x00);
extern Register VC12_HI = Register(RegisterAddress::VC12_HI, 0x00);
extern Register VC12_LO = Register(RegisterAddress::VC12_LO, 0x00);
extern Register VC13_HI = Register(RegisterAddress::VC13_HI, 0x00);
extern Register VC13_LO = Register(RegisterAddress::VC13_LO, 0x00);
extern Register VC14_HI = Register(RegisterAddress::VC14_HI, 0x00);
extern Register VC14_LO = Register(RegisterAddress::VC14_LO, 0x00);
extern Register VC15_HI = Register(RegisterAddress::VC15_HI, 0x00);
extern Register VC15_LO = Register(RegisterAddress::VC15_LO, 0x00);



extern Register VCELL[30] = {
    VC1_HI, VC1_LO,
    VC2_HI, VC2_LO,
    VC3_HI, VC3_LO,
    VC4_HI, VC4_LO,
    VC5_HI, VC5_LO,
    VC6_HI, VC6_LO,
    VC7_HI, VC7_LO,
    VC8_HI, VC8_LO,
    VC9_HI, VC9_LO,
    VC10_HI, VC10_LO,
    VC11_HI, VC11_LO,
    VC12_HI, VC12_LO,
    VC13_HI, VC13_LO,
    VC14_HI, VC14_LO,
    VC15_HI, VC15_LO
};


uint16_t VCELL_getVoltage(uint8_t cell_index);


extern Register BAT_HI = Register(RegisterAddress::BAT_HI, 0x00);
extern Register BAT_LO = Register(RegisterAddress::BAT_LO, 0x00);

uint16_t BAT_getVoltage();

#endif  //  REGISTERSVC_HPP