#ifndef CAN_HELPER_HPP
#define CAN_HELPER_HPP

#include <stdint.h>
#include "AmsState.hpp"
#include "Calculator.hpp"

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
#define NUM_SLAVE_FRAME 6

class CanHelper
{
    public:
        uint16_t PANIC_ADDRESS = MCP2515_PANIC_ADDRESS;
        uint16_t MASTER_ADDRESS = MCP2515_MASTER_ADDRESS;
        uint16_t SLAVE_ADDRESS = MCP2515_SLAVE_ADDRESS;

        CanHelper(AmsState &ams_, MCP2515 &mcp2515_, Calculator &calculator_);
        CanHelper() = delete;
        void setNodeID(bool &jp1, bool &jp2, bool &jp3, bool &jp4);
        uint16_t getSlaveAddress(uint8_t index);
        void sendMasterData(uint8_t index);
        void sendPanic();
        void packMasterData(can_frame &frame);

    private:
        AmsState &ams;
        MCP2515 &mcp2515;
        Calculator &calculator;
};

//  Master Command Byte bits
#define MASTERCMD_DISCHARGE_STATE_BIT   0x01
#define MASTERCMD_CELLBAL_STATE_BIT     0x02
#define MASTERCMD_CELLBAL_ODD_BIT       0x04
#define MASTERCMD_SEQTOGGLE_BIT         0x08

#endif  //  CAN_HELPER_HPP