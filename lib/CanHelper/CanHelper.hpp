#ifndef CAN_HELPER_HPP
#define CAN_HELPER_HPP

#include <stdint.h>
#include "AmsState.hpp"

// ignore -Wpedantic warnings for mcp2515.h
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <mcp2515.h>
#pragma GCC diagnostic pop

//  MCP2515 Address
#define MCP2515_PANIC_ADDRESS 0x100
#define MCP2515_MASTER_ADDRESS 0x200
#define MCP2515_SLAVE_ADDRESS 0x300

#define NUM_SLAVE 10
#define NUM_SLAVE_FRAME 5

class CanHelper
{
    public:
        uint16_t PANIC_ADDRESS = MCP2515_PANIC_ADDRESS;
        uint16_t MASTER_ADDRESS = MCP2515_MASTER_ADDRESS;
        uint16_t SLAVE_ADDRESS = MCP2515_SLAVE_ADDRESS;

        constexpr CanHelper(AmsState &ams_state_, MCP2515 &mcp2515_);
        CanHelper() = delete;
        void setNodeID(bool &jp1, bool &jp2, bool &jp3, bool &jp4);
        uint16_t getSlaveAddress(uint8_t index);
        void sendVoltages(uint8_t index);
        void sendPanic();

    private:
        AmsState &ams_state;
        MCP2515 &mcp2515;
};

#endif  //  CAN_HELPER_HPP