#ifndef ADC_CALIBRATION_HPP
#define ADC_CALIBRATION_HPP

#include <stdint.h>

constexpr uint16_t VOLTAGE_THRESHOLD = 4200; // millivolts


/**
 * @class Bq76940AdcCalibration
 * @brief Unifies and manages the factory-programmed ADC calibration registers.
 *
 * @details The BQ76940 features production-fused calibration factors to maximize 
 * the measurement accuracy of its 14-bit core ADC. Due to silicon architecture constraints, 
 * the 5-bit ADC Gain value is split across two non-contiguous registers (0x50 and 0x59), 
 * while the 8-bit ADC Offset resides as an independent token in register 0x51.
 * * This class abstracts the hardware scattering by holding all raw inputs and exposing 
 * mathematically unified constants ready for raw-voltage conversion formulas.
 */
class Bq76940AdcCalibration {
private:
    // --- Physical Hardware I2C Addresses ---
    static constexpr uint8_t ADDR_GAIN1   = 0x50; /**< I2C address for the upper ADC Gain bits */
    static constexpr uint8_t ADDR_OFFSET  = 0x51; /**< I2C address for the signed ADC Offset byte */
    static constexpr uint8_t ADDR_GAIN2   = 0x59; /**< I2C address for the lower ADC Gain bits */

    // --- Bitmasks ---
    
    /**
     * @brief Mask for Bits 3-2 in ADCGAIN1 (Address 0x50).
     * * These factory-trimmed bits represent **Bit 4 (MSB)** and **Bit 3** of the 
     * complete 5-bit ADC Gain scaling factor.
     */
    static constexpr uint8_t GAIN1_MASK   = 0x0C;

    /**
     * @brief Mask for Bits 7-5 in ADCGAIN2 (Address 0x59).
     * * These factory-trimmed bits represent **Bit 2**, **Bit 1**, and **Bit 0 (LSB)** * of the complete 5-bit ADC Gain scaling factor.
     */
    static constexpr uint8_t GAIN2_MASK   = 0xE0;

    // --- Raw Hardware Storage ---
    uint8_t gain1_raw;  /**< Stores raw byte read from register 0x50 */
    uint8_t gain2_raw;  /**< Stores raw byte read from register 0x59 */
    uint8_t offset_raw; /**< Stores raw byte read from register 0x51 */

public:
    /**
     * @brief Construct a new Bq76940 Adc Calibration object initialized to zero.
     */
    Bq76940AdcCalibration() : gain1_raw(0x00), gain2_raw(0x00), offset_raw(0x00) {}

    /**
     * @brief Bulk loads the raw data bytes acquired from the physical I2C read commands.
     * @param g1 The raw 8-bit payload read from register address 0x50.
     * @param g2 The raw 8-bit payload read from register address 0x59.
     * @param offset The raw 8-bit payload read from register address 0x51.
     */
    void loadRegisters(uint8_t g1, uint8_t g2, uint8_t offset) {
        gain1_raw  = g1;
        gain2_raw  = g2;
        offset_raw = offset;
    }

    // --- I2C Address Getters for Bus Driver Querying ---
    uint8_t getGain1Address() const  { return ADDR_GAIN1; }
    uint8_t getGain2Address() const  { return ADDR_GAIN2; }
    uint8_t getOffsetAddress() const { return ADDR_OFFSET; }

    /**
     * @brief Extracts and reassembles the mathematical 5-bit ADC Gain factor.
     * * @details This function isolates the scattered bit fields, shifts them into 
     * alignment, and merges them.
     * - Bits 3-2 from GAIN1 are shifted left by 1 to occupy target bits 4-3.
     * - Bits 7-5 from GAIN2 are shifted right by 5 to occupy target bits 2-0.
     * * @return uint8_t A unified 5-bit integer (range 0 to 31) representing the absolute ADC Gain.
     */
    uint8_t getCombinedGain() const {
        uint8_t upper_bits = (gain1_raw & GAIN1_MASK) << 1; 
        uint8_t lower_bits = (gain2_raw & GAIN2_MASK) >> 5; 
        return (upper_bits | lower_bits);
    }

    /** CHANGE THE DOXYGEN COMMENT HERE
     * @brief Translates the raw baseline ADC offset value.
     * * @details Unlike the gain component, the offset occupies an undivided 8-bit field. 
     * However, it is fundamentally encoded as a **Two's Complement signed integer**. 
     * This method safely forces the signed cast to preserve negative offset values.
     * * @return int8_t The signed calibration offset value in microvolts/counts.
     */
    uint8_t getSignedOffset() const {
        return static_cast<int8_t>(offset_raw);
    }

    // Helper Functions, Explained in VoltageThreshold.cpp
    uint16_t ADCGain() const;
    int8_t ADCOffset() const;
};

#endif // ADC_CALIBRATION_HPP