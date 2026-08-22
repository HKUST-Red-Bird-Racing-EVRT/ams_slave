#ifndef REGISTERS_CLASS_HPP
#define REGISTERS_CLASS_HPP

#include <stdint.h>

enum class RegisterAddress : uint8_t
{
    SYS_STAT = 0x00,
    SYS_CTRL1 = 0x04,
    // SYS_CTRL2 = 0x05,
    // PROTECT1 = 0x06,
    // PROTECT2 = 0x07,
    PROTECT3 = 0x08,
    OV_TRIP = 0x09,
    UV_TRIP = 0x0A,
    // CC_CFG = 0x0B,
    VC1_HI = 0x0C,
    VC1_LO = 0x0D,
    VC2_HI = 0x0E,
    VC2_LO = 0x0F,
    VC3_HI = 0x10,
    VC3_LO = 0x11,
    VC4_HI = 0x12,
    VC4_LO = 0x13,
    VC5_HI = 0x14,
    VC5_LO = 0x15,
    VC6_HI = 0x16,
    VC6_LO = 0x17,
    VC7_HI = 0x18,
    VC7_LO = 0x19,
    VC8_HI = 0x1A,
    VC8_LO = 0x1B,
    VC9_HI = 0x1C,
    VC9_LO = 0x1D,
    VC10_HI = 0x1E,
    VC10_LO = 0x1F,
    VC11_HI = 0x20,
    VC11_LO = 0x21,
    VC12_HI = 0x22,
    VC12_LO = 0x23,
    VC13_HI = 0x24,
    VC13_LO = 0x25,
    VC14_HI = 0x26,
    VC14_LO = 0x27,
    VC15_HI = 0x28,
    VC15_LO = 0x29,
    BAT_HI = 0x2A,
    BAT_LO = 0x2B,
    TS1_HI = 0x2C,
    TS1_LO = 0x2D,
    TS2_HI = 0x2E,
    TS2_LO = 0x2F,
    TS3_HI = 0x30,
    TS3_LO = 0x31,
    ADCGAIN1 = 0x50,
    ADCOFFSET = 0x51,
    ADCGAIN2 = 0x59
};

class Register
{

private:
    const RegisterAddress ADDRESS; /**< The physical I2C register address for this register. */
    uint8_t raw_data; /**< The raw 8-bit data value stored in this register. */

public:
    Register(RegisterAddress address, uint8_t data)
        : ADDRESS(address),
          raw_data(data) {}

    void load(uint8_t data) { raw_data = data; }
    RegisterAddress getAddress() const { return ADDRESS; }
    uint8_t getRawData() const { return raw_data; }
};

#endif // REGISTERS_CLASS_HPP