#ifndef REGISTERS_CLASS_HPP
#define REGISTERS_CLASS_HPP

#include <stdint.h>

class Register
{

private:
    const uint8_t ADDRESS; /**< The physical I2C register address for this register. */
    uint8_t raw_data; /**< The raw 8-bit data value stored in this register. */

public:
    Register(uint8_t address, uint8_t data)
        : ADDRESS(address),
          raw_data(data) {}

    void load(uint8_t data) { raw_data = data; }
    uint8_t getAddress() const { return ADDRESS; }
    uint8_t getRawData() const { return raw_data; }
};

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
    // VCELL1 = 0x0C,
    // VCELL2 = 0x0E,
    // VCELL3 = 0x10,
    // VCELL4 = 0x12,
    // VCELL5 = 0x14,
    // VCELL6 = 0x16,
    ADCGAIN1 = 0x50,
    ADCOFFSET = 0x51,
    ADCGAIN2 = 0x59
};

#endif // REGISTERS_CLASS_HPP