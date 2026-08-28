#ifndef AMSSTATE_HPP
#define AMSSTATE_HPP

#include <stdint.h>

constexpr uint8_t NUM_VC = 14; /**< Number of cells monitored by this AMS slave node. */
constexpr uint8_t NUM_TS = 5; /**< Number of temperature sensors monitored by this AMS slave node. */

/**
 * @brief Represents the local state of one AMS slave node.
 *
 * This structure holds the measurements and status information for a single
 * section of the accumulator, including cell voltages, temperature readings,
 * fault/status flags, balancing state, and communication bookkeeping.
 * @param cell_voltages             An array of voltage readings for the cells monitored by this slave.
 * @param temperatures              An array of temperature readings from local thermistors or sensors.
 * @param fault_flags               A bitfield representing various fault and status conditions such as overvoltage, undervoltage, or sensor faults.
 * @param node_id                   An identifier for this slave node on the AMS network.
 * @param packet_counter            A counter for received or transmitted communication packets, used for tracking message sequence.
 */

struct AmsState
{
    uint16_t cell_voltages[NUM_VC];   /**< Cell voltage readings for the cells monitored by this slave. */
    uint32_t message_timestamp = 0;       /**< Timestamp of the last valid message received from this slave. */
    uint16_t temperatures[NUM_TS]; /**< Temperature readings from local thermistors. */

    uint8_t command_flags = 0x00; /**< Command flag received from the master, used to control slave behavior. */

    uint32_t cellbal_timestamp = 0; /**< Timestamp of the last cell balancing operation. */
    bool cellbal_active = false;         /**< CellBal State*/
    bool is_balancevoltage_recieved = false; /**< Flag indicating if the balance voltage has been received. */
    bool cellbal_odd = false;   /**< CellBal Odd/Even State*/
    uint16_t cellbal_flags = 0x00;

    bool discharge_active = false; /**< Discharging State*/
    bool fault_active = false;
    uint8_t fault_flags = 0x00;   /**< Fault and status bits such as overvoltage, undervoltage, or sensor faults. */

    uint8_t node_id;         /**< Identifier for this slave node on the AMS network. */
    uint8_t packet_counter;  /**< Counter for received or transmitted communication packets. */
};

/*
Command Flag Bits
Bit 0: discharge_state
Bit 1: cellbal_state
Bit 2: cellbal_odd
Bit 3: sequence_toggle
*/

#define DISCHARGE_STATE_BIT         0x01
#define CELLBAL_STATE_BIT           0x02
#define CELLBAL_ODD_BIT             0x04
#define SEQUENCE_TOGGLE_BIT         0x08

//  CellBal Mask Bits
#define CELLBAL_EVEN_MASK           0x1555 // 0001 0101 0101 0101
#define CELLBAL_ODD_MASK            0x4AAA // 0100 1010 1010 1010

//  Fault Flag Bits
#define OVERVOLTAGE_FAULT_BIT       0x01
#define UNDERVOLTAGE_FAULT_BIT      0x02
#define OVERTEMPERATURE_FAULT_BIT   0x04
#define UNDERTEMPERATURE_FAULT_BIT  0x08
#define I2C_TIMEOUT_FAULT_BIT       0x10

#endif // AMSSTATE_HPP