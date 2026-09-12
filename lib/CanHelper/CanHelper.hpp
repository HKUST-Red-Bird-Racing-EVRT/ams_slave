/**
 * @file CanHelper.hpp
 * @brief Public interface for AMS slave CAN communication.
 */

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

/** @brief Number of slave nodes supported by the CAN protocol. */
#define NUM_SLAVE 10
/** @brief Number of CAN data frames used to report one slave state. */
#define NUM_SLAVE_FRAME 6

/** @brief Encodes and sends AMS slave CAN messages. */
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
    /** Master command base address; the node ID is added during setup. */
    uint16_t MASTER_COMMAND_ADDRESS = 0x200;
    /** Master request base address for the six response frames. */
    uint16_t MASTER_REQUEST_ADDRESS = 0x210;
    /** Slave response base address; the node ID is added during setup. */
    uint16_t SLAVE_MESSAGE_ADDRESS = 0x300;

    /**
     * @brief Creates a CAN helper.
     * @param ams_ State object read from and updated by the helper.
     * @param mcp2515_ MCP2515 controller used for CAN transfers.
     * @param calculator_ Calculator used when applying received commands.
     */
    CanHelper(AmsState &ams_, MCP2515 &mcp2515_, Calculator &calculator_);
    /** @brief Prevents construction without the required dependencies. */
    CanHelper() = delete;

    /** @brief Sets the node ID from the four hardware jumper inputs. */
    void setNodeID(bool &jp1, bool &jp2, bool &jp3, bool &jp4);

    /** @param index Response frame index. @return CAN address for that frame. */
    uint16_t getSlaveMessageAddress(uint8_t index);

    /** @param address Master request address. @return Corresponding response index. */
    uint8_t getMessageIndexFromAddress(uint16_t address);

    /** @brief Sends the requested voltage or temperature frame to the master. */
    void sendMasterData(uint8_t index);

    /** @brief Sends the current fault flags to the master. */
    void sendPanic();

    /** @brief Applies a command frame received from the master. */
    void packMasterData(can_frame &frame);

private:
    AmsState &ams;
    MCP2515 &mcp2515;
    Calculator &calculator;
};

/** @brief Master command byte bit masks. */
#define MASTERCMD_DISCHARGE_STATE_BIT 0x01
#define MASTERCMD_CELLBAL_STATE_BIT 0x02
#define MASTERCMD_CELLBAL_ODD_BIT 0x04
#define MASTERCMD_SEQTOGGLE_BIT 0x08

#endif //  CAN_HELPER_HPP