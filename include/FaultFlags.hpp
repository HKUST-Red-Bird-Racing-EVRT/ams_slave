#ifndef FAULT_FLAGS_HPP
#define FAULT_FLAGS_HPP

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
        uint8_t getFlags() const {return flags;};

        bool getOverallFault() const;
        void setOverallFault();
        void clearOverallFault();

        bool getOvervoltageFault() const;
        void setOvervoltageFault();
        void clearOvervoltageFault();

        bool getUndervoltageFault() const;
        void setUndervoltageFault();
        void clearUndervoltageFault();

        bool getOvertemperatureFault() const;
        void setOvertemperatureFault();
        void clearOvertemperatureFault();

        bool getUndertemperatureFault() const;
        void setUndertemperatureFault();
        void clearUndertemperatureFault();

        bool getI2CTimeoutFault() const;
        void setI2CTimeoutFault();
        void clearI2CTimeoutFault();
};
#endif  // FAULT_FLAGS_HPP