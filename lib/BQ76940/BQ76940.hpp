#ifndef BQ76940_HPP
#define BQ76940_HPP

#include "Registers.hpp"
#include "AmsState.hpp"
#include "I2C.hpp"

#include <stdint.h>

template <uint16_t BITRATE_KBPS, uint8_t PRIORITY_SIZE, uint8_t RECURRING_SIZE, uint8_t WATCHDOG_MAX_COUNT>
class BQ76940
{
    public:
        constexpr BQ76940(AmsState &ams_state_, I2C<BITRATE_KBPS, PRIORITY_SIZE, RECURRING_SIZE, WATCHDOG_MAX_COUNT> &i2c_);
        BQ76940() = delete; // Prevent default construction without AmsState reference
        void readVoltage();

    private:
        AmsState &ams_state;
        I2C<BITRATE_KBPS, PRIORITY_SIZE, RECURRING_SIZE, WATCHDOG_MAX_COUNT> &i2c;
        static constexpr uint8_t BQ76940_I2C_ADDRESS = 0x08; /**< I2C address for the BQ76940 device. */
        static constexpr uint8_t BQ76940_VOLTAGE_REGISTER = 0x0C; /**< Register address for reading cell voltages. */
        static constexpr uint8_t VOLTAGE_READ_COUNT = 30; /**< Number of bytes to read voltage data for. */
        uint8_t voltage_buffer[VOLTAGE_READ_COUNT]; /**< Buffer to hold raw voltage data read from the BQ76940. Each cell voltage is 2 bytes. */

        const I2cTransaction voltage_write_transaction = I2cTransaction::makeWrite(BQ76940_I2C_ADDRESS, 1, &BQ76940_VOLTAGE_REGISTER);
        const I2cTransaction voltage_read_transaction = I2cTransaction::makeRead(BQ76940_I2C_ADDRESS, VOLTAGE_READ_COUNT, voltage_buffer);
};

#endif // BQ76940_HPP