#ifndef REGISTERSPROTECT_HPP
#define REGISTERSPROTECT_HPP

#include "RegistersClass.hpp"
#include <stdint.h>

/**
 * @class RegisterPROTECT3
 * @brief Encapsulates the Protection 3 Configuration Register (Address: 0x08).
 *
 * @details Configures the physical hardware debounce and validation delays 
 * before cell fault protections switch off the safety switch networks.
 */

 enum class BitPROTECT3 : uint8_t
{
    /**
     * @brief Bits 7-6: Undervoltage Delay Configuration (UV_D1, UV_D0).
     * * Controls the qualification time filter window for cell undervoltage.
     * - 00 = 1 second
     * - 01 = 2 seconds
     * - 10 = 4 seconds
     * - 11 = 8 seconds
     */
    UV_D1 = 1 << 7,
    UV_D0 = 1 << 6,

    /**
     * @brief Bits 5-4: Overvoltage Delay Configuration (OV_D1, OV_D0).
     * * Controls the qualification time filter window for cell overvoltage.
     * - 00 = 1 second
     * - 01 = 2 seconds
     * - 10 = 4 seconds
     * - 11 = 8 seconds
     */
    OV_D1 = 1 << 5,
    OV_D0 = 1 << 4
};

extern Register PROTECT3 = Register(static_cast<uint8_t>(RegisterAddress::PROTECT3), 0x00);

uint8_t PROTECT3_getUVDelayCode();
uint8_t PROTECT3_getOVDelayCode();

#endif  // REGISTERSPROTECT_HPP