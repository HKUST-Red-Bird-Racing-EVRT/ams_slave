#ifndef REGISTERS_HPP
#define REGISTERS_HPP

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
class RegisterSYS_STAT {
private:
    static constexpr uint8_t REG_ADDR = 0x00;

    /**
     * @brief Bit 7: Coulomb Counter Ready Flag (CC_READY).
     * * Set by the AFE every 250 ms to signal that a new current integration 
     * measurement loop has completed. It indicates that updated current and 
     * accumulated charge data are ready to be read from the CC registers.
     */
    static constexpr uint8_t CC_READY      = 1 << 7;

    /**
     * @brief Bit 5: Internal Device Fault Flag (DEVICE_XREADY).
     * * Indicates an internal chip diagnostic failure. This can be triggered by 
     * an LDO regulator fault, a failure of the internal digital oscillator core, 
     * or a critical startup diagnostic timeout. Normal voltage conversions 
     * are suspended while this bit is set.
     */
    static constexpr uint8_t DEVICE_XREADY = 1 << 5;

    /**
     * @brief Bit 4: External Override Alert Flag (OVRD_ALERT).
     * * Asserted when an external component, secondary protection IC, or the 
     * host microcontroller physically pulls the hardware ALERT pin high. This 
     * allows the main MCU to handle system-driven external interrupt signals 
     * through the AFE's reporting structure.
     */
    static constexpr uint8_t OVRD_ALERT    = 1 << 4;

    /**
     * @brief Bit 3: Cell Undervoltage Fault Flag (UV).
     * * Asserted when any individual cell voltage drops below the value defined 
     * in the UV_TRIP register for a continuous duration exceeding the 
     * configured UV_DELAY window. Triggers the immediate internal hardware 
     * safety disable of the discharge path (DSG FET).
     */
    static constexpr uint8_t UV            = 1 << 3;

    /**
     * @brief Bit 2: Cell Overvoltage Fault Flag (OV).
     * * Asserted when any individual cell voltage climbs above the threshold 
     * defined in the OV_TRIP register for a continuous duration exceeding the 
     * configured OV_DELAY window. Triggers the immediate internal hardware 
     * safety disable of the charge path (CHG FET).
     */
    static constexpr uint8_t OV            = 1 << 2;

    /**
     * @brief Bit 1: Short Circuit in Discharge Fault Flag (SCD).
     * * An ultra-fast hardware protection latch. Triggered instantaneously when 
     * the differential voltage drop across the current sense resistor exceeds 
     * the configured hardware short-circuit threshold. Disables the DSG driver 
     * within microseconds to protect the power stage.
     */
    static constexpr uint8_t SCD           = 1 << 1;

    /**
     * @brief Bit 0: Overcurrent in Discharge Fault Flag (OCD).
     * * Triggered when discharge current through the external sense resistor 
     * exceeds the overcurrent threshold configuration for longer than the programmed 
     * delay window. Deactivates the DSG FET driver.
     */
    static constexpr uint8_t OCD           = 1 << 0;

    uint8_t raw_data;

public:
    RegisterSYS_STAT() : raw_data(0x00) {}

    void load(uint8_t data) { raw_data = data; }
    uint8_t getAddress() const { return REG_ADDR; }
    uint8_t getRaw() const { return raw_data; }

    bool getCCReady() const       { return (raw_data & CC_READY) != 0; }
    bool getDeviceFault() const   { return (raw_data & DEVICE_XREADY) != 0; }
    bool getOverrideAlert() const { return (raw_data & OVRD_ALERT) != 0; }
    bool getUndervoltage() const   { return (raw_data & UV) != 0; }
    bool getOvervoltage() const    { return (raw_data & OV) != 0; }
    bool getShortCircuit() const  { return (raw_data & SCD) != 0; }
    bool getOvercurrent() const    { return (raw_data & OCD) != 0; }
};


/**
 * @class RegisterSYS_CTRL1
 * @brief Encapsulates the System Control 1 Register (Address: 0x04).
 *
 * @details Governs power management profiles, main ADC subsystems, temperature 
 * acquisition options, and system state shutdown transitions.
 */
class RegisterSYS_CTRL1 {
private:
    static constexpr uint8_t REG_ADDR = 0x04;

    /** CHANGE THE DOXYGEN COMMENT HERE
     * @brief Bit 7: Coulomb Counter Enable (CC_EN).
     * * Controls the operational state of the current-sensing ADC. 
     * - 1 = Continuous integration mode enabled (samples constantly, logs every 250ms).
     * - 0 = Disabled, placing the current sensor block into low-power sleep mode.
     */
    static constexpr uint8_t LOAD_PRESENT = 1 << 7;

    /**
     * @brief Bit 4: Main ADC Enable (ADC_EN).
     * * Activates the 14-bit main monitoring Analog-to-Digital Converter.
     * - 1 = Enabled. The ADC steps sequentially through all attached cell voltage paths 
     * and thermal acquisition configurations.
     * - 0 = Disabled. Halts automated loops to reduce active current consumption.
     */
    static constexpr uint8_t ADC_EN    = 1 << 4;

    /**
     * @brief Bit 3: Temperature Sensor Select (TEMP_SEL).
     * * Toggles the diagnostic input multiplexer feeding the internal ADC.
     * - 1 = External NTC thermistor channels (TS1, TS2, TS3) are sampled.
     * - 0 = Internal silicon die temperature sensor is sampled.
     */
    static constexpr uint8_t TEMP_SEL  = 1 << 3;

    /**
     * @brief Bit 1: Shutdown Control Bit A (SHUT_A).
     * @brief Bit 0: Shutdown Control Bit B (SHUT_B).
     * * These bits must be manipulated sequentially by the host controller to safely 
     * push the AFE into an ultra-low power storage state (SHIP Mode). 
     * Shifting into SHIP mode deactivates all internal LDO regulators, FET 
     * drivers, and communications.
     * * Sequence: Host writes SHUT_B = 1, then writes SHUT_A = 1 sequentially.
     */
    static constexpr uint8_t SHUT_A    = 1 << 1;
    static constexpr uint8_t SHUT_B    = 1 << 0;

    uint8_t raw_data;

public:
    RegisterSYS_CTRL1() : raw_data(0x00) {}
    
    void load(uint8_t data) { raw_data = data; }
    uint8_t getAddress() const { return REG_ADDR; }
    
    bool getLoadPresent() const        { return (raw_data & LOAD_PRESENT) != 0; }
    bool getADCEnabled() const       { return (raw_data & ADC_EN) != 0; }
    bool getExternalTempSel() const { return (raw_data & TEMP_SEL) != 0; }
};


/**
 * @class RegisterPROTECT3
 * @brief Encapsulates the Protection 3 Configuration Register (Address: 0x08).
 *
 * @details Configures the physical hardware debounce and validation delays 
 * before cell fault protections switch off the safety switch networks.
 */
class RegisterPROTECT3 {
private:
    static constexpr uint8_t REG_ADDR = 0x08;

    /**
     * @brief Bits 7-6: Cell Undervoltage Delay Bits (UV_D1, UV_D0).
     * * Controls the qualification time filter window for cell undervoltage.
     * - 00 = 1 second
     * - 01 = 4 seconds
     * - 10 = 8 seconds
     * - 11 = 16 seconds
     */
    static constexpr uint8_t UV_D1         = 1 << 7;
    static constexpr uint8_t UV_D0         = 1 << 6;

    /**
     * @brief Bits 5-4: Cell Overvoltage Delay Bits (OV_D1, OV_D0).
     * * Controls the qualification time filter window for cell overvoltage.
     * - 00 = 1 second
     * - 01 = 2 seconds
     * - 10 = 4 seconds
     * - 11 = 8 seconds
     */
    static constexpr uint8_t OV_D1         = 1 << 5;
    static constexpr uint8_t OV_D0         = 1 << 4;

    uint8_t raw_data;

public:
    RegisterPROTECT3() : raw_data(0x00) {}
    
    void load(uint8_t data) { raw_data = data; }
    uint8_t getAddress() const { return REG_ADDR; }

    uint8_t getUVDelayCode() const { return (raw_data & 0xC0) >> 6; }
    uint8_t getOVDelayCode() const { return (raw_data & 0x30) >> 4; }
};


/**
 * @class RegisterOV_TRIP
 * @brief Encapsulates the Overvoltage Trip Threshold Register (Address: 0x09).
 *
 * @details Bits 7-0 represent a full 8-bit digital target mapping configuration value.
 * Used alongside the factory calibration ADC gain and offset settings to set 
 * the hardware cell overvoltage comparison threshold.
 */
class RegisterOV_TRIP {
private:
    static constexpr uint8_t REG_ADDR = 0x09;
    uint8_t raw_data;

public:
    RegisterOV_TRIP() : raw_data(0xAC) {}

    void load(uint8_t data) { raw_data = data; }
    uint8_t getAddress() const { return REG_ADDR; }
    uint8_t getThreshold() const { return raw_data; }
};


/**
 * @class RegisterUV_TRIP
 * @brief Encapsulates the Undervoltage Trip Threshold Register (Address: 0x0A).
 *
 * @details Bits 7-0 map the raw baseline targets used by internal comparison circuits 
 * to flag systemic over-discharge safety thresholds. Like OV_TRIP, it acts 
 * as a cohesive 8-bit hardware setting.
 */
class RegisterUV_TRIP {
private:
    static constexpr uint8_t REG_ADDR = 0x0A;
    uint8_t raw_data;

public:
    RegisterUV_TRIP() : raw_data(0x97) {}

    void load(uint8_t data) { raw_data = data; }
    uint8_t getAddress() const { return REG_ADDR; }
    uint8_t getThreshold() const { return raw_data; }
};


#endif  // REGISTERS_HPP