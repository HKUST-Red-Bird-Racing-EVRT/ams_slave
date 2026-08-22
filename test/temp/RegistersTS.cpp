#include "RegistersTS.hpp"

uint16_t TSENSOR_getVoltage(uint8_t sensor_index)
{
    if (sensor_index >= 3)
    {
        return 0; // Invalid sensor index
    }

    uint8_t high_byte = TSENSOR[sensor_index * 2].getRawData();
    uint8_t low_byte = TSENSOR[sensor_index * 2 + 1].getRawData();

    return ((static_cast<uint16_t>(high_byte) & 0x3F) << 8) | low_byte;
}