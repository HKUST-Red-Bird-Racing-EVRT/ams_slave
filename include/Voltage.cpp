#include "Voltage.hpp"
#include "AmsState.hpp"

void checkAndSetVoltageFaults(AmsState &state)
{
    for (uint8_t cell_index = 0; cell_index < 15; ++cell_index)
    {
        uint16_t cell_voltage = VCELL_getVoltage(cell_index);

        if (cell_voltage < UV_TRIP_THRESHOLD_FULL)
        {
            // Handle undervoltage fault for this cell
            // For example, log the fault, trigger an alert, or take corrective action

            state.fault_flags.setUndervoltageFault();
            messager.sendPanic(state.fault_flags);
        }
        else if (cell_voltage > OV_TRIP_THRESHOLD_FULL)
        {
            // Handle overvoltage fault for this cell
            // For example, log the fault, trigger an alert, or take corrective action

            state.fault_flags.setOvervoltageFault();
            messager.sendPanic(state.fault_flags);

            
        }
    }
}