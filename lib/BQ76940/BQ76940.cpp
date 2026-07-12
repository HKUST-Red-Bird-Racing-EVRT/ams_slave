#include "BQ76940.hpp"
#include "AmsState.hpp"
#include "I2C.hpp"
#include "RegistersSYS.hpp"
#include "RegistersVC.hpp"
#include "RegistersTS.hpp"
#include <Arduino.h>
#include <stdint.h>

void BQ76940::setSHIPMode()
{

    if (SYS_CTRL1_getShipModeEnabled())
    {
        current_mode = FunctionalMode::SHIP;
    }
    else
    {
        current_mode = FunctionalMode::NORMAL;
    }
}

void BQ76940::setNORMALMode()
{
    if (TSENSOR_getVoltage(0) >= VOLTAGE_BOOT)
    {
        if (booton_timestamp == 0)
        {
            booton_timestamp = millis();
        }
        else
        {
            if (millis() - booton_timestamp >= TIME_BOOT)
            {
                current_mode = FunctionalMode::NORMAL;
            }
        }
    }
}

void BQ76940::readVoltage()
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
                    ams_state.cell_voltages[i] = ((static_cast<uint16_t>(high_byte) & 0x3F) << 8) | low_byte;
                }

                else
                {
                    uint8_t high_byte = voltage_buffer[(i + 1) * 2];
                    uint8_t low_byte = voltage_buffer[(i + 1) * 2 + 1];
                    ams_state.cell_voltages[i] = ((static_cast<uint16_t>(high_byte) & 0x3F) << 8) | low_byte;
                }
            }
        }
    }
}