#include "BQ76940.hpp"
template <uint16_t BITRATE_KBPS, uint8_t PRIORITY_SIZE, uint8_t RECURRING_SIZE, uint8_t WATCHDOG_MAX_COUNT>
constexpr BQ76940<BITRATE_KBPS, PRIORITY_SIZE, RECURRING_SIZE, WATCHDOG_MAX_COUNT>::BQ76940(
    AmsState &ams_,
    I2C<BITRATE_KBPS, PRIORITY_SIZE, RECURRING_SIZE, WATCHDOG_MAX_COUNT> &i2c_,
    Calculator &calculator_)
    : ams(ams_),
      i2c(i2c_),
      calculator(calculator_)
{
}

template <uint16_t BITRATE_KBPS, uint8_t PRIORITY_SIZE, uint8_t RECURRING_SIZE, uint8_t WATCHDOG_MAX_COUNT>
void BQ76940<BITRATE_KBPS, PRIORITY_SIZE, RECURRING_SIZE, WATCHDOG_MAX_COUNT>::readVoltage()
{
    if (i2c.pushRecurring(voltage_write_transaction))
    {
        if (i2c.pushRecurring(voltage_read_transaction))
        {
            // now we queued new round of reads, buffers themselves hold new data just read
            // copy data out from buffer
            // then exit function to allow i2c to read another round of data

            uint8_t high_byte, low_byte;

            for (uint8_t i = 0; i < NUM_VC; ++i)
            {
                if (i <= 12) // only read 13 cells, the 14th is not used
                {
                    high_byte = voltage_buffer[i * 2];
                    low_byte = voltage_buffer[i * 2 + 1];
                }

                else
                {
                    high_byte = voltage_buffer[(i + 1) * 2];
                    low_byte = voltage_buffer[(i + 1) * 2 + 1];
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

template <uint16_t BITRATE_KBPS, uint8_t PRIORITY_SIZE, uint8_t RECURRING_SIZE, uint8_t WATCHDOG_MAX_COUNT>
uint8_t BQ76940<BITRATE_KBPS, PRIORITY_SIZE, RECURRING_SIZE, WATCHDOG_MAX_COUNT>::getRegisterReadData(uint8_t address)
{
    uint8_t buffer[1] = {0};

	const uint8_t send_addess[1] = {address};


    const I2cTransaction write_transaction = I2cTransaction::makeWrite(IC_ADDRESS, 1, send_addess);
    const I2cTransaction read_transaction = I2cTransaction::makeRead(IC_ADDRESS, 1, buffer); 
	while (!i2c.pushPriority(write_transaction))
		;


	while (!i2c.pushPriority(read_transaction))
		;
	i2c.pump();
    delay(100);

    return buffer[0];
}

template <uint16_t BITRATE_KBPS, uint8_t PRIORITY_SIZE, uint8_t RECURRING_SIZE, uint8_t WATCHDOG_MAX_COUNT>
void BQ76940<BITRATE_KBPS, PRIORITY_SIZE, RECURRING_SIZE, WATCHDOG_MAX_COUNT>::writeCellBal()
{
    uint8_t cellbal_flags_low = ams.cellbal_flags & 0x1F;
    uint8_t cellbal_flags_mid = (ams.cellbal_flags >> 5) & 0x1F;
    uint8_t cellbal_flags_high = (ams.cellbal_flags >> 10) & 0x1F;

    const uint8_t send_cellbal1_data[2] = {REGISTER_CELLBAL1_ADDRESS, cellbal_flags_low};

    const I2cTransaction cellbal1_write_transaction = I2cTransaction::makeWrite(IC_ADDRESS, 1, send_cellbal1_data);

    while (!i2c.pushPriority(cellbal1_write_transaction))
        ;

    const uint8_t send_cellbal2_data[2] = {REGISTER_CELLBAL2_ADDRESS, cellbal_flags_mid};

    const I2cTransaction cellbal2_write_transaction = I2cTransaction::makeWrite(IC_ADDRESS, 1, send_cellbal2_data);

    while (!i2c.pushPriority(cellbal2_write_transaction))
        ;

    const uint8_t send_cellbal3_data[2] = {REGISTER_CELLBAL3_ADDRESS, cellbal_flags_high};

    const I2cTransaction cellbal3_write_transaction = I2cTransaction::makeWrite(IC_ADDRESS, 1, send_cellbal3_data);

    while (!i2c.pushPriority(cellbal3_write_transaction))
        ;
}