/**
 * @file BQ76940.tpp
 * @brief Template implementation for the BQ76940 interface.
 */

#include "BQ76940.hpp"
template <uint16_t BITRATE_KBPS, uint8_t QUEUE_SIZE, uint8_t WATCHDOG_MAX_COUNT>
constexpr BQ76940<BITRATE_KBPS, QUEUE_SIZE, WATCHDOG_MAX_COUNT>::BQ76940(
    AmsState &ams_,
    I2C<BITRATE_KBPS, QUEUE_SIZE, WATCHDOG_MAX_COUNT> &i2c_,
    Calculator &calculator_)
    : ams(ams_),
      i2c(i2c_),
      calculator(calculator_)
{
}

template <uint16_t BITRATE_KBPS, uint8_t QUEUE_SIZE, uint8_t WATCHDOG_MAX_COUNT>
void BQ76940<BITRATE_KBPS, QUEUE_SIZE, WATCHDOG_MAX_COUNT>::readVoltage()
{
    if (i2c.push(voltage_write_transaction))
    {
        if (i2c.push(voltage_read_transaction))
        {
            // now we queued new round of reads, buffers themselves hold new data just read
            // copy data out from buffer
            // then exit function to allow i2c to read another round of data

            uint8_t local_voltage_buffer[VOLTAGE_READ_COUNT];
            memcpy(local_voltage_buffer, voltage_buffer, VOLTAGE_READ_COUNT);
            uint8_t high_byte, low_byte;

            for (uint8_t i = 0; i < NUM_VC; ++i)
            {
                if (i <= 12) // only read 13 cells, the 14th is not used
                {
                    high_byte = local_voltage_buffer[i * 2];
                    low_byte = local_voltage_buffer[i * 2 + 1];
                }

                else
                {
                    high_byte = local_voltage_buffer[(i + 1) * 2];
                    low_byte = local_voltage_buffer[(i + 1) * 2 + 1];
                }

                uint16_t voltage = ((static_cast<uint16_t>(high_byte) & 0x3F) << 8) | low_byte;
                uint16_t real_voltage = calculator.getRealVoltage(voltage);
                ams.cell_voltages[i] = real_voltage;

                if (real_voltage >= calculator.VOLTAGE_MAX && !ams.discharge_active)
                {
                    ams.cellbal_flags |= CELLBAL_STATE_BIT;
                }

            }
        }
    }
}

template <uint16_t BITRATE_KBPS, uint8_t QUEUE_SIZE, uint8_t WATCHDOG_MAX_COUNT>
uint8_t BQ76940<BITRATE_KBPS, QUEUE_SIZE, WATCHDOG_MAX_COUNT>::getRegisterReadData(uint8_t address)
{
    uint8_t buffer[1] = {0};

	const uint8_t send_addess[1] = {address};


    const I2cTransaction write_transaction = I2cTransaction::makeWrite(IC_ADDRESS, 1, send_addess);
    const I2cTransaction read_transaction = I2cTransaction::makeRead(IC_ADDRESS, 1, buffer); 
	while (!i2c.push(write_transaction))
		;


	while (!i2c.push(read_transaction))
		;

    // while queue still have things:
    while (!i2c.queueEmpty())
    {
        i2c.pump();
    }

    return buffer[0];
}

template <uint16_t BITRATE_KBPS, uint8_t QUEUE_SIZE, uint8_t WATCHDOG_MAX_COUNT>
void BQ76940<BITRATE_KBPS, QUEUE_SIZE, WATCHDOG_MAX_COUNT>::writeCellBal()
{
    uint8_t cellbal_flags_low = ams.cellbal_flags & 0x1F;
    uint8_t cellbal_flags_mid = (ams.cellbal_flags >> 5) & 0x1F;
    uint8_t cellbal_flags_high = (ams.cellbal_flags >> 10) & 0x1F;

    const uint8_t send_cellbal1_data[2] = {REGISTER_CELLBAL1_ADDRESS, cellbal_flags_low};

    const I2cTransaction cellbal1_write_transaction = I2cTransaction::makeWrite(IC_ADDRESS, 1, send_cellbal1_data);

    while (!i2c.push(cellbal1_write_transaction))
        ;

    const uint8_t send_cellbal2_data[2] = {REGISTER_CELLBAL2_ADDRESS, cellbal_flags_mid};

    const I2cTransaction cellbal2_write_transaction = I2cTransaction::makeWrite(IC_ADDRESS, 1, send_cellbal2_data);

    while (!i2c.push(cellbal2_write_transaction))
        ;

    const uint8_t send_cellbal3_data[2] = {REGISTER_CELLBAL3_ADDRESS, cellbal_flags_high};

    const I2cTransaction cellbal3_write_transaction = I2cTransaction::makeWrite(IC_ADDRESS, 1, send_cellbal3_data);

    while (!i2c.push(cellbal3_write_transaction))
        ;
}