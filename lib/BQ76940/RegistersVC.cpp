#include "RegistersVC.hpp"

uint16_t VCELL_getIndividualVoltage(uint8_t cell_index)
{
    if (cell_index >= 15)
    {
        return 0; // Invalid cell index
    }

    uint8_t high_byte = VCELL[cell_index * 2].getRawData();
    uint8_t low_byte = VCELL[cell_index * 2 + 1].getRawData();

    return ((static_cast<uint16_t>(high_byte) & 0x3F) << 8) | low_byte;
}

uint16_t VCELL_getGroupVoltage(uint8_t group_index)
{
    if (group_index >= 3)
    {
        return 0; // Invalid group index
    }

    uint16_t group_voltage = 0;
    for (uint8_t i = 0; i < 5; ++i)
    {
        uint8_t cell_index = group_index * 5 + i;
        if (cell_index < 15) // Ensure we don't exceed the number of cells
        {
            group_voltage += VCELL_getIndividualVoltage(cell_index);
        }
    }

    return group_voltage;
}

uint16_t BAT_getVoltage()
{
    uint8_t high_byte = BAT_HI.getRawData();
    uint8_t low_byte = BAT_LO.getRawData();

    return ((static_cast<uint16_t>(high_byte) & 0x3F) << 8) | low_byte;
}