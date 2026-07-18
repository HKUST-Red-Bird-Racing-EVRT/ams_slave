#include "AmsState.hpp"
#include "FaultFlags.hpp"

/*
void AmsState::checkVoltage()
{
    for (uint8_t cell_index = 0; cell_index < NUM_VC; ++cell_index)
    {
        uint16_t cell_voltage = cell_voltages[cell_index];

        if (cell_voltage < UV_TRIP_THRESHOLD_FULL)
        {
            fault_flags.setUndervoltageFault();
            messager.sendPanic(fault_flags);
        }
        else if (cell_voltage > OV_TRIP_THRESHOLD_FULL)
        {
            fault_flags.setOvervoltageFault();
            messager.sendPanic(fault_flags);
        }
    }
}

void AmsState::checkTemperature()
{
    for (uint8_t temp_index = 0; temp_index < NUM_TS; ++temp_index)
    {
        uint16_t temperature = temperatures[temp_index];

        if (temperature > VOLTAGE_OT)
        {
            fault_flags.setOvertemperatureFault();
            messager.sendPanic(fault_flags);
        }
        else if (temperature < VOLTAGE_UT)
        {
            fault_flags.setUndertemperatureFault();
            messager.sendPanic(fault_flags);
        }
    }
}
*/