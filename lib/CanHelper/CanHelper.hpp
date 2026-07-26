#ifndef CAN_HELPER_HPP
#define CAN_HELPER_HPP

#include <stdint.h>
#include "AmsState.hpp"

//  MCP2515 Address
#define MCP2515_PANIC_ADDRESS 0x100
#define MCP2515_MASTER_ADDRESS 0x200
#define MCP2515_SLAVE_ADDRESS 0x300

class CanHelper
{
    public:
        uint16_t PANIC_ADDRESS = MCP2515_PANIC_ADDRESS;
        uint16_t MASTER_ADDRESS = MCP2515_MASTER_ADDRESS;
        uint16_t SLAVE_ADDRESS = MCP2515_SLAVE_ADDRESS;

        constexpr CanHelper(AmsState &ams_state_);
        CanHelper() = delete;
        void setNodeID(bool &jp1, bool &jp2, bool &jp3, bool &jp4);

    private:
        AmsState &ams_state;
};

#endif  //  CAN_HELPER_HPP