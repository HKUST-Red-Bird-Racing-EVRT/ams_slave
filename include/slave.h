#ifndef SLAVE_H
#define SLAVE_H

#include <stdint.h>
#include "enums.h"

/**
 * @file Slave.hpp
 * @brief Defines the AMS slave local state container.
 *
 * This file contains the data structure used to store the local measurements
 * and status values managed by one AMS slave node.
 */

/**
 * @brief Represents the local state of one AMS slave node.
 *
 * This structure holds the measurements and status information for a single
 * section of the accumulator, including cell voltages, temperature readings,
 * fault/status flags, balancing state, and communication bookkeeping.
 * @param cell_voltages             An array of voltage readings for the cells monitored by this slave.
 * @param adc_temperatures          An array of temperature ADC readings from local thermistors or sensors
 * @param fault_flags               A bitfield representing various fault and status conditions such as overvoltage, undervoltage, or sensor faults.
 * @param balance_state             A bitfield representing the cell balancing state or bleed mask for the local section.
 * @param node_id                   An identifier for this slave node on the AMS network.
 * @param packet_counter            A counter for received or transmitted communication packets, used for tracking message sequence.
 * @param last_valid_message        A boolean indicating whether the most recent message received from this slave was valid, used for communication health monitoring.
 */


struct slave_state
{
    uint16_t cell_voltages[14];    /**< Cell voltage readings for the cells monitored by this slave. */
    uint16_t adc_temperatures[5];   /**< Temperature ADC readings from local thermistors or sensors. */

    uint16_t fault_flags;           /**< Fault and status bits such as overvoltage, undervoltage, or sensor faults. */
    uint16_t balance_state;         /**< Cell balancing state or bleed mask for the local section. */

    uint8_t node_id;                /**< Identifier for this slave node on the AMS network. */
    uint8_t packet_counter;         /**< Counter for received or transmitted communication packets. */
    bool last_valid_message;        /**< True if the most recent message received from this slave was valid. */
};

#endif // SLAVE_H