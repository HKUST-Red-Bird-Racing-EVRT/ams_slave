#include "Temperature.hpp"

void checkAndSetTemperatureFaults(AmsState &state)
{
    for (uint8_t temp_index = 0; temp_index < 5; ++temp_index)
    {
        uint16_t temperature = state.temperatures[temp_index];

        if (temperature > VOLTAGE_OT)
        {
            // Handle overtemperature fault for this sensor
            // For example, log the fault, trigger an alert, or take corrective action

            state.fault_flags.setOvertemperatureFault();
            messager.sendPanic(state.fault_flags);
        }
        else if (temperature < VOLTAGE_UT)
        {
            // Handle undertemperature fault for this sensor
            // For example, log the fault, trigger an alert, or take corrective action

            state.fault_flags.setUndertemperatureFault();
            messager.sendPanic(state.fault_flags);
        }
    }
}