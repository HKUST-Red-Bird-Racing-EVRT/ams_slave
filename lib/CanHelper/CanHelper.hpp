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

#define NUM_SLAVE 10
#define NUM_SLAVE_FRAME 6

class CanHelper
{
public:
    //  MCP2515 CAN Address
    /*
    Panic Address: 0x1xx
    slave 0: 0x100, slave 1: 0x101, slave 2: 0x102, ..., slave 9: 0x109

    Master Address: 0x2xx
    slave 0: 0x200, slave 1: 0x201, slave 2: 0x202, ..., slave 9: 0x209 (send command data to slaves)
    frame 0: 0x210, frame 1: 0x211, frame 2: 0x212, ..., frame 5: 0x215 (request data from slave)
    (send 1 frame of each slaves together, 6 frames total)

    Slave Address: 0x3xx
    slave 0: 0x3x0, slave 1: 0x3x1, slave 2: 0x3x2, ..., slave 9: 0x3x9
    frame 0: 0x30x, frame 1: 0x31x, frame 2: 0x32x, ..., frame 5: 0x35x
    */

    uint16_t PANIC_MESSAGE_ADDRESS = 0x100;
    uint16_t MASTER_COMMAND_ADDRESS = 0x200;
    uint16_t MASTER_REQUEST_ADDRESS = 0x210;
    uint16_t SLAVE_MESSAGE_ADDRESS = 0x300;

    CanHelper(AmsState &ams_, MCP2515 &mcp2515_, Calculator &calculator_);
    CanHelper() = delete;
    void setNodeID(bool &jp1, bool &jp2, bool &jp3, bool &jp4);
    uint16_t getSlaveMessageAddress(uint8_t index);
    uint8_t getMessageIndexFromAddress(uint16_t address);
    void sendMasterData(uint8_t index);
    void sendPanic();
    void packMasterData(can_frame &frame);

private:
    AmsState &ams;
    MCP2515 &mcp2515;
    Calculator &calculator;
};

//  Master Command Byte bits
#define MASTERCMD_DISCHARGE_STATE_BIT 0x01
#define MASTERCMD_CELLBAL_STATE_BIT 0x02
#define MASTERCMD_CELLBAL_ODD_BIT 0x04
#define MASTERCMD_SEQTOGGLE_BIT 0x08

#endif //  CAN_HELPER_HPP