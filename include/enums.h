#ifndef ENUMS_H
#define ENUMS_H

#include <stdint.h>

/**
 * @brief Fault flags reported by an AMS slave.
 *
 * This enum defines the individual fault conditions that can be detected by
 * the AMS slave. Each value represents one bit in a bitmask, so multiple
 * faults can be stored in the same `uint16_t` variable by combining them with
 * the bitwise OR operator (`|`).
 *
 * Example:
 * @code
 * uint16_t fault_flags = FAULT_OVERVOLTAGE | FAULT_COMMUNICATION;
 * @endcode
 *
 * The bitmask can later be checked with the bitwise AND operator (`&`).
 * Example:
 * @code
 * if (fault_flags & FAULT_OVERVOLTAGE) {
 *     // Overvoltage fault is present
 * }
 * @endcode
 */


enum SlaveFault : uint16_t
{
    /** No fault is active. */
    FAULT_NONE = 0x0000,

    /** One or more monitored cell voltages are above the allowed limit. */
    FAULT_OVERVOLTAGE = 0x0001,

    /** One or more monitored cell voltages are below the allowed limit. */
    FAULT_UNDERVOLTAGE = 0x0002,

    /** One or more temperature sensors report a temperature above the limit. */
    FAULT_OVERTEMPERATURE = 0x0004,

    /** A temperature sensor or measurement line appears to be open circuit. */
    FAULT_SENSOR_OPEN = 0x0008,

    /** A temperature sensor or measurement line appears to be short circuit. */
    FAULT_SENSOR_SHORT = 0x0010,

    /** The slave has a communication error or the last message was not valid. */
    FAULT_COMMUNICATION = 0x0020,
};

#endif // ENUMS_H