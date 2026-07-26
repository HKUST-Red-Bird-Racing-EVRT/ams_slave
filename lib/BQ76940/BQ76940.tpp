#include "BQ76940.hpp"
template <uint16_t BITRATE_KBPS, uint8_t PRIORITY_SIZE, uint8_t RECURRING_SIZE, uint8_t WATCHDOG_MAX_COUNT>
constexpr BQ76940<BITRATE_KBPS, PRIORITY_SIZE, RECURRING_SIZE, WATCHDOG_MAX_COUNT>::BQ76940(
    AmsState &ams_state_,
    I2C<BITRATE_KBPS, PRIORITY_SIZE, RECURRING_SIZE, WATCHDOG_MAX_COUNT> &i2c_,
    Calculator &calculator_)
    : ams_state(ams_state_),
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

            for (uint8_t i = 0; i < NUM_VC; ++i)
            {
                if (i <= 12) // only read 13 cells, the 14th is not used
                {
                    uint8_t high_byte = voltage_buffer[i * 2];
                    uint8_t low_byte = voltage_buffer[i * 2 + 1];
                    uint16_t voltage = ((static_cast<uint16_t>(high_byte) & 0x3F) << 8) | low_byte;
                    ams_state.cell_voltages[i] = calculator.getRealVoltage(voltage);
                }

                else
                {
                    uint8_t high_byte = voltage_buffer[(i + 1) * 2];
                    uint8_t low_byte = voltage_buffer[(i + 1) * 2 + 1];
                    uint16_t voltage = ((static_cast<uint16_t>(high_byte) & 0x3F) << 8) | low_byte;
                    ams_state.cell_voltages[i] = calculator.getRealVoltage(voltage);
                }
            }
        }
    }
}