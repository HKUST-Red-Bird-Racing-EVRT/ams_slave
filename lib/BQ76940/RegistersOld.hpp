#ifndef REGISTERS_OLD_HPP
#define REGISTERS_OLD_HPP

#include <stdint.h>

/**
 * @brief Encapsulates the System Status (SYS_STAT) register of the BQ76940.
 *
 * @details This class provides safe, read-only access to hardware faults and 
 * manages the specific bitwise operations required to clear them. It hides the 
 * raw hex masks and memory addresses to prevent accidental memory corruption.
 * * @note Hardware Rule: To clear a specific fault flag in the BQ76940, the 
 * microcontroller must write a '1' back to the corresponding bit. 
 */

class RegisterSYS_STAT {
    
    private:

    /** @brief The physical I2C register address for SYS_STAT */
    static const uint8_t REG_ADDR = 0x00;

    // --- Private Hardware Masks ---
    static const uint8_t CC_READY      = 1 << 7;     /**< Bit 7: Coulomb Counter Ready (New current reading available) */
    static const uint8_t RSVD          = 1 << 6;     /**< Bit 6: Reserved (Do not use) */    
    static const uint8_t DEVICE_XREADY = 1 << 5;     /**< Bit 5: Internal Device Fault flag */
    static const uint8_t OVRD_ALERT    = 1 << 4;     /**< Bit 4: External Override Alert flag */
    static const uint8_t UV            = 1 << 3;     /**< Bit 3: Undervoltage fault flag */
    static const uint8_t OV            = 1 << 2;     /**< Bit 2: Overvoltage fault flag */
    static const uint8_t SCD           = 1 << 1;     /**< Bit 1: Short Circuit in Discharge fault flag */
    static const uint8_t OCD           = 1 << 0;     /**< Bit 0: Overcurrent in Discharge fault flag */

    // The actual data payload hidden from the outside world
    uint8_t raw_data;

    public:
    RegisterSYS_STAT(){ raw_data = 0x00; };

};

class RegisterSYS_CTRL1 {
    
    private:

    /** @brief The physical I2C register address for SYS_CTRL1 */
    static const uint8_t REG_ADDR = 0x04;

    // --- Private Hardware Masks ---
    static const uint8_t CC_EN         = 1 << 7;     /**< Bit 7: Coulomb Counter Enable */
    static const uint8_t RSVD          = 1 << 6;     /**< Bit 6: Reserved (Do not use) */
    static const uint8_t RSVD         = 1 << 5;     /**< Bit 5: Reserved (Do not use) */
    static const uint8_t ADC_EN        = 1 << 4;     /**< Bit 4: ADC Enable */
    static const uint8_t TEMP_SEL      = 1 << 3;     /**< Bit 3: Reserved (Do not use) */
    static const uint8_t RSVD         = 1 << 2;     /**< Bit 2: Reserved (Do not use) */
    static const uint8_t SHUT_A        = 1 << 1;     /**< Bit 1: Reserved (Do not use) */
    static const uint8_t SHUT_B        = 1 << 0;     /**< Bit 0: Reserved (Do not use) */

    
    uint8_t raw_data;

    public:
    RegisterSYS_CTRL1(){ raw_data = 0x00; };
};

class RegisterPROTECT3 {
    
    private:

    /** @brief The physical I2C register address for PROTECT3 */
    static const uint8_t REG_ADDR = 0x08;

    // --- Private Hardware Masks ---
    static const uint8_t UV_D1         = 1 << 7;     /**< Bit 7: Reserved (Do not use) */
    static const uint8_t UV_D0         = 1 << 6;     /**< Bit 6: Reserved (Do not use) */
    static const uint8_t OV_D1         = 1 << 5;     /**< Bit 5: Reserved (Do not use) */
    static const uint8_t OV_D0         = 1 << 4;     /**< Bit 4: Reserved (Do not use) */
    static const uint8_t RSVD          = 1 << 3;     /**< Bit 3: Reserved (Do not use) */
    static const uint8_t RSVD         = 1 << 2;     /**< Bit 2: Reserved (Do not use) */
    static const uint8_t RSVD         = 1 << 1;     /**< Bit 1: Reserved (Do not use) */
    static const uint8_t RSVD         = 1 << 0;     /**< Bit 0: Reserved (Do not use) */

    
    uint8_t raw_data;

    public:
    RegisterPROTECT3(){ raw_data = 0x00; };
};

class RegisterOV_TRIP {
    
    private:

    /** @brief The physical I2C register address for OV_TRIP */
    static const uint8_t REG_ADDR = 0x09;

    // --- Private Hardware Masks ---
    #define OV_T7  1 << 7;     /**< Bits 7-0: Overvoltage Trip Threshold */
    #define OV_T6  1 << 6;     /**< Bits 7-0: Overvoltage Trip Threshold */
    #define OV_T5  1 << 5;     /**< Bits 7-0: Overvoltage Trip Threshold */
    #define OV_T4  1 << 4;     /**< Bits 7-0: Overvoltage Trip Threshold */
    #define OV_T3  1 << 3;     /**< Bits 7-0: Overvoltage Trip Threshold */
    #define OV_T2  1 << 2;     /**< Bits 7-0: Overvoltage Trip Threshold */
    #define OV_T1  1 << 1;     /**< Bits 7-0: Overvoltage Trip Threshold */
    #define OV_T0  1 << 0;     /**< Bits 7-0: Overvoltage Trip Threshold */

    
    uint8_t raw_data;

    public:
    RegisterOV_TRIP(){ raw_data = 0x00; };
};

class RegisterUV_TRIP {
    
    private:

    /** @brief The physical I2C register address for UV_TRIP */
    static const uint8_t REG_ADDR = 0x0A;

    // --- Private Hardware Masks ---
    #define UV_T7  1 << 7;     /**< Bits 7-0: Undervoltage Trip Threshold */
    #define UV_T6  1 << 6;     /**< Bits 7-0: Undervoltage Trip Threshold */
    #define UV_T5  1 << 5;     /**< Bits 7-0: Undervoltage Trip Threshold */
    #define UV_T4  1 << 4;     /**< Bits 7-0: Undervoltage Trip Threshold */
    #define UV_T3  1 << 3;     /**< Bits 7-0: Undervoltage Trip Threshold */
    #define UV_T2  1 << 2;     /**< Bits 7-0: Undervoltage Trip Threshold */
    #define UV_T1  1 << 1;     /**< Bits 7-0: Undervoltage Trip Threshold */
    #define UV_T0  1 << 0;     /**< Bits 7-0: Undervoltage Trip Threshold */

    
    uint8_t raw_data;

    public:
    RegisterUV_TRIP(){ raw_data = 0x00; };
};

class RegisterADCGAIN1 {
    
    private:

    /** @brief The physical I2C register address for ADCGAIN */
    static const uint8_t REG_ADDR = 0x50;

    // --- Private Hardware Masks ---
    #define RSVD  1 << 7;           /**< Bits 7: Reserved */
    #define RSVD  1 << 6;           /**< Bits 6: Reserved */
    #define RSVD  1 << 5;           /**< Bits 5: Reserved */
    #define RSVD  1 << 4;           /**< Bits 4: Reserved */
    #define ADCGAIN4  1 << 3;       /**< Bits 3: ADC Gain Setting */
    #define ADCGAIN3  1 << 2;       /**< Bits 2: ADC Gain Setting */
    #define RSVD  1 << 1;           /**< Bits 1: Reserved */
    #define RSVD  1 << 0;           /**< Bits 0: Reserved */

    
    uint8_t raw_data;

    public:
    RegisterADCGAIN1(){ raw_data = 0x00; };
};

class RegisterADCGAIN2 {
    
    private:

    /** @brief The physical I2C register address for ADCGAIN */
    static const uint8_t REG_ADDR = 0x59;

    // --- Private Hardware Masks ---
    #define ADCGAIN2  1 << 7;           /**< Bits 7: ADC Gain Setting */
    #define ADCGAIN1  1 << 6;           /**< Bits 6: ADC Gain Setting */
    #define ADCGAIN0  1 << 5;           /**< Bits 5: ADC Gain Setting */
    #define RSVD  1 << 4;               /**< Bits 4: Reserved */
    #define RSVD  1 << 3;               /**< Bits 3: Reserved */
    #define RSVD  1 << 2;               /**< Bits 2: Reserved */
    #define RSVD  1 << 1;               /**< Bits 1: Reserved */
    #define RSVD  1 << 0;               /**< Bits 0: Reserved */

    
    uint8_t raw_data;

    public:
    RegisterADCGAIN2(){ raw_data = 0x00; };
};

class RegisterADCOFFSET {
    
    private:

    /** @brief The physical I2C register address for ADCOFFSET */
    static const uint8_t REG_ADDR = 0x51;

    // --- Private Hardware Masks ---
    #define ADCOFFSET7  1 << 7;           /**< Bits 7-0: ADC Offset Setting */
    #define ADCOFFSET6  1 << 6;           /**< Bits 7-0: ADC Offset Setting */
    #define ADCOFFSET5  1 << 5;           /**< Bits 7-0: ADC Offset Setting */
    #define ADCOFFSET4  1 << 4;           /**< Bits 7-0: ADC Offset Setting */
    #define ADCOFFSET3  1 << 3;           /**< Bits 7-0: ADC Offset Setting */
    #define ADCOFFSET2  1 << 2;           /**< Bits 7-0: ADC Offset Setting */
    #define ADCOFFSET1  1 << 1;           /**< Bits 7-0: ADC Offset Setting */
    #define ADCOFFSET0  1 << 0;           /**< Bits 7-0: ADC Offset Setting */

    
    uint8_t raw_data;

    public:
    RegisterADCOFFSET(){ raw_data = 0x00; };
};

#endif  // REGISTERS_OLD_HPP