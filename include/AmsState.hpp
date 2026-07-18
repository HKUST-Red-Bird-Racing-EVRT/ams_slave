#ifndef AMSSTATE_HPP
#define AMSSTATE_HPP

#include "FaultFlags.hpp"
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
    uint32_t timestamp;              /**< Timestamp of the last valid message received from this slave. */
    uint16_t temperatures[NUM_TS]; /**< Temperature readings from local thermistors. */

    FaultFlags fault_flags;   /**< Fault and status bits such as overvoltage, undervoltage, or sensor faults. */

    uint8_t node_id;         /**< Identifier for this slave node on the AMS network. */
    uint8_t packet_counter;  /**< Counter for received or transmitted communication packets. */

    // void checkVoltage();
    // void checkTemperature();
};

#endif // AMSSTATE_HPP