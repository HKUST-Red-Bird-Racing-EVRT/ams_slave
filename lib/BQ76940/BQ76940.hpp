/**
 * @file BQ76940.hpp
 * @brief Public interface for reading and controlling the BQ76940 monitor.
 */

#ifndef BQ76940_HPP
#define BQ76940_HPP

#include "AmsState.hpp"
#include "BqRegsiters.h"
#include "Calculator.hpp"
#include "I2C.hpp"
#include <Arduino.h>
#include <stdint.h>

/**
 * @brief Interface to the BQ76940 battery monitor.
 *
 * @tparam BITRATE_KBPS I2C bus bitrate in kbps.
 * @tparam QUEUE_SIZE Number of entries in the I2C transaction queue.
 * @tparam WATCHDOG_MAX_COUNT Maximum number of inactive I2C pump cycles.
 */
template <uint16_t BITRATE_KBPS, uint8_t QUEUE_SIZE, uint8_t WATCHDOG_MAX_COUNT>
class BQ76940
{
    public:
        /**
         * @brief Creates a BQ76940 interface.
         *
         * @param ams_ State object updated with measurements and status.
         * @param i2c_ I2C driver used to communicate with the monitor.
         * @param calculator_ Calculator used to convert raw measurements.
         */
        constexpr BQ76940(AmsState &ams_, I2C<BITRATE_KBPS, QUEUE_SIZE, WATCHDOG_MAX_COUNT> &i2c_, Calculator &calculator_);

        /** @brief Prevents construction without the required dependencies. */
        BQ76940() = delete;

        /** @brief Queues a cell-voltage read and updates the AMS state. */
        void readVoltage();

        /**
         * @brief Reads one register from the BQ76940.
         * @param address Register address to read.
         * @return Register value returned by the device.
         */
        uint8_t getRegisterReadData(uint8_t address);

        /** @brief Writes the current cell-balancing flags to the device. */
        void writeCellBal();

    private:
        AmsState &ams;
        I2C<BITRATE_KBPS, QUEUE_SIZE, WATCHDOG_MAX_COUNT> &i2c;
        Calculator &calculator;
        static constexpr uint8_t BQ76940_I2C_ADDRESS = IC_ADDRESS; /**< I2C address for the BQ76940 device. */

        static constexpr uint8_t BQ76940_VOLTAGE_REGISTER = REGISTER_VC1_HI_ADDRESS; /**< Register address for reading cell voltages. */
        static constexpr uint8_t VOLTAGE_READ_COUNT = 30; /**< Number of bytes to read voltage data for. */
        uint8_t voltage_buffer[VOLTAGE_READ_COUNT]; /**< Buffer to hold raw voltage data read from the BQ76940. Each cell voltage is 2 bytes. */
        uint32_t booton_timestamp = 0; /**< Timestamp when the device was powered on, used for boot timing. */

        const I2cTransaction voltage_write_transaction = I2cTransaction::makeWrite(BQ76940_I2C_ADDRESS, 1, &BQ76940_VOLTAGE_REGISTER);
        const I2cTransaction voltage_read_transaction = I2cTransaction::makeRead(BQ76940_I2C_ADDRESS, VOLTAGE_READ_COUNT, voltage_buffer);
};

#include "BQ76940.tpp"

#endif // BQ76940_HPP