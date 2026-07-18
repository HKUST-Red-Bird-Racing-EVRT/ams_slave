#ifndef REGISTERSSYS_HPP
#define REGISTERSSYS_HPP

#include "RegistersClass.hpp"
#include "RegistersVC.hpp"
#include "Voltage.hpp"
#include <stdint.h>

/**
 * @class RegisterSYS_STAT
 * @brief Encapsulates the System Status (SYS_STAT) Register (Address: 0x00).
 *
 * @details This register reports latching hardware faults, operational alerts, 
 * and data conversion readiness states. 
 *
 * @note Hardware Rule: Bits in this register are cleared by writing a '1' to 
 * the target bit from the host microcontroller. Writing a '0' has no effect.
 */

enum class BitSYS_STAT : uint8_t
{
    /**
     * @brief Bit 7: Coulomb Counter Ready Flag (CC_READY).
     * * Set by the AFE every 250 ms to signal that a new current integration 
     * measurement loop has completed. It indicates that updated current and 
     * accumulated charge data are ready to be read from the CC registers.
     */
    CC_READY = 1 << 7,

    /**
     * @brief Bit 5: Internal Device Fault Flag (DEVICE_XREADY).
     * * Indicates an internal chip diagnostic failure. This can be triggered by 
     * an LDO regulator fault, a failure of the internal digital oscillator core, 
     * or a critical startup diagnostic timeout. Normal voltage conversions 
     * are suspended while this bit is set.
     */
    DEVICE_XREADY = 1 << 5,

    /**
     * @brief Bit 4: External Override Alert Flag (OVRD_ALERT).
     * * Asserted when an external component, secondary protection IC, or the 
     * host microcontroller physically pulls the hardware ALERT pin high. This 
     * allows the main MCU to handle system-driven external interrupt signals 
     * through the AFE's reporting structure.
     */
    OVRD_ALERT = 1 << 4,

    /**
     * @brief Bit 3: Cell Undervoltage Fault Flag (UV).
     * * Asserted when any individual cell voltage drops below the value defined 
     * in the UV_TRIP register for a continuous duration exceeding the 
     * configured UV_DELAY window. Triggers the immediate internal hardware 
     * safety disable of the discharge path (DSG FET).
     */
    UV = 1 << 3,

    /**
     * @brief Bit 2: Cell Overvoltage Fault Flag (OV).
     * * Asserted when any individual cell voltage climbs above the threshold 
     * defined in the OV_TRIP register for a continuous duration exceeding the 
     * configured OV_DELAY window. Triggers the immediate internal hardware 
     * safety disable of the charge path (CHG FET).
     */
    OV = 1 << 2,

    /**
     * @brief Bit 1: Short Circuit in Discharge Fault Flag (SCD).
     * * An ultra-fast hardware protection latch. Triggered instantaneously when 
     * the differential voltage drop across the current sense resistor exceeds 
     * the configured hardware short-circuit threshold. Disables the DSG driver 
     * within microseconds to protect the power stage.
     */
    SCD = 1 << 1,

    /**
     * @brief Bit 0: Overcurrent in Discharge Fault Flag (OCD).
     * * Triggered when discharge current through the external sense resistor 
     * exceeds the overcurrent threshold configuration for longer than the programmed 
     * delay window. Deactivates the DSG FET driver.
     */
    OCD = 1 << 0
};

inline Register SYS_STAT = Register(RegisterAddress::SYS_STAT, 0x00);

void SYS_STAT_setCCReady(bool value);
bool SYS_STAT_getCCReady();

void SYS_STAT_setInternalDeviceFault(bool value);
bool SYS_STAT_getInternalDeviceFault();

void SYS_STAT_setExternalOverrideAlert(bool value);
bool SYS_STAT_getExternalOverrideAlert();

void SYS_STAT_setUndervoltageFault(bool value);
bool SYS_STAT_getUndervoltageFault();

void SYS_STAT_setOvervoltageFault(bool value);
bool SYS_STAT_getOvervoltageFault();

void SYS_STAT_setShortCircuitFault(bool value);
bool SYS_STAT_getShortCircuitFault();

void SYS_STAT_setOvercurrentFault(bool value);
bool SYS_STAT_getOvercurrentFault();


/**
 * @class RegisterSYS_CTRL1
 * @brief Encapsulates the System Control 1 Register (Address: 0x04).
 *
 * @details Governs power management profiles, main ADC subsystems, temperature 
 * acquisition options, and system state shutdown transitions.
 */

enum class BitSYS_CTRL1 : uint8_t
{
    /** CHANGE THE DOXYGEN COMMENT HERE
     * @brief Bit 7: Coulomb Counter Enable (CC_EN).
     * * Controls the operational state of the current-sensing ADC. 
     * - 1 = Continuous integration mode enabled (samples constantly, logs every 250ms).
     * - 0 = Disabled, placing the current sensor block into low-power sleep mode.
     */
    LOAD_PRESENT = 1 << 7,

    /**
     * @brief Bit 4: Main ADC Enable (ADC_EN).
     * * Activates the 14-bit main monitoring Analog-to-Digital Converter.
     * - 1 = Enabled. The ADC steps sequentially through all attached cell voltage paths 
     * and thermal acquisition configurations.
     * - 0 = Disabled. Halts automated loops to reduce active current consumption.
     */
    ADC_EN = 1 << 4,

    /**
     * @brief Bit 3: Temperature Sensor Select (TEMP_SEL).
     * * Toggles the diagnostic input multiplexer feeding the internal ADC.
     * - 1 = External NTC thermistor channels (TS1, TS2, TS3) are sampled.
     * - 0 = Internal silicon die temperature sensor is sampled.
     */
    TEMP_SEL = 1 << 3,

    /**
     * @brief Bit 1: Shutdown Control Bit A (SHUT_A).
     * @brief Bit 0: Shutdown Control Bit B (SHUT_B).
     * * These bits must be manipulated sequentially by the host controller to safely 
     * push the AFE into an ultra-low power storage state (SHIP Mode). 
     * Shifting into SHIP mode deactivates all internal LDO regulators, FET 
     * drivers, and communications.
     * * Sequence: Host writes SHUT_B = 1, then writes SHUT_A = 1 sequentially.
     */
    SHUT_A = 1 << 1,
    SHUT_B = 1 << 0
};

inline Register SYS_CTRL1 = Register(RegisterAddress::SYS_CTRL1, 0x00);

bool SYS_CTRL1_getLoadPresent();
bool SYS_CTRL1_getADCEnabled();
bool SYS_CTRL1_getExternalTempSel();
bool SYS_CTRL1_getShutA();
bool SYS_CTRL1_getShutB();

#endif // REGISTERSSYS_HPP