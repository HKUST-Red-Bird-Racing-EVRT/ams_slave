#ifndef FAULT_FLAGS_HPP
#define FAULT_FLAGS_HPP

#include "Registers.hpp"
#include <stdint.h>

/*  1 byte represents fault status, 
    The LSB is for any fault The bits following are:
    Overvoltage
    Undervoltage
    Overtemperature
    Undertemperature
    I2C Timeout


    Function: Get, Set, Clear
*/

class FaultFlags
{
    private:
        uint8_t flags;

        #define OVERALL 0x00
        #define OVERVOLTAGE 0x01
        #define UNDERVOLTAGE 0x02
        #define OVERTEMPERATURE 0x04
        #define UNDERTEMPERATURE 0x08
        #define I2C_TIMEOUT 0x10

    public:
    
        FaultFlags() : flags(0) {}

        bool getOverallFault() const { return flags & OVERALL; };
        void setOverallFault() { flags |= OVERALL; };
        void clearOverallFault() { flags &= ~OVERALL; };

        bool getOvervoltageFault() const { return flags & OVERVOLTAGE; };
        void setOvervoltageFault() { flags |= OVERVOLTAGE; };
        void clearOvervoltageFault() { flags &= ~OVERVOLTAGE; };

        bool getUndervoltageFault() const { return flags & UNDERVOLTAGE; };
        void setUndervoltageFault() { flags |= UNDERVOLTAGE; };
        void clearUndervoltageFault() { flags &= ~UNDERVOLTAGE; };

        bool getOvertemperatureFault() const { return flags & OVERTEMPERATURE; };
        void setOvertemperatureFault() { flags |= OVERTEMPERATURE; };
        void clearOvertemperatureFault() { flags &= ~OVERTEMPERATURE; };

        bool getUndertemperatureFault() const { return flags & UNDERTEMPERATURE; };
        void setUndertemperatureFault() { flags |= UNDERTEMPERATURE; };
        void clearUndertemperatureFault() { flags &= ~UNDERTEMPERATURE; };

        bool getI2CTimeoutFault() const { return flags & I2C_TIMEOUT; };
        void setI2CTimeoutFault() { flags |= I2C_TIMEOUT; };
        void clearI2CTimeoutFault() { flags &= ~I2C_TIMEOUT; };
};
#endif  // FAULT_FLAGS_HPP