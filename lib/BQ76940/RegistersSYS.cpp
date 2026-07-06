#include "RegistersSYS.hpp"

//  Register SYS_STAT Functions

bool SYS_STAT_getCCReady()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::CC_READY)) != 0;
}

bool SYS_STAT_getInternalDeviceFault()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::DEVICE_XREADY)) != 0;
}

bool SYS_STAT_getExternalOverrideAlert()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::OVRD_ALERT)) != 0;
}

bool SYS_STAT_getUndervoltageFault()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::UV)) != 0;
}

bool SYS_STAT_getOvervoltageFault()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::OV)) != 0;
}

bool SYS_STAT_getShortCircuitFault()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::SCD)) != 0;
}

bool SYS_STAT_getOvercurrentFault()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::OCD)) != 0;
}

//  Register SYS_CTRL1 Functions

bool SYS_CTRL1_getLoadPresent()
{
    return (SYS_CTRL1.getRawData() & static_cast<uint8_t>(BitSYS_CTRL1::LOAD_PRESENT)) != 0;
}

bool SYS_CTRL1_getADCEnabled(){
    return (SYS_CTRL1.getRawData() & static_cast<uint8_t>(BitSYS_CTRL1::ADC_EN)) != 0;
}

bool SYS_CTRL1_getExternalTempSel(){
    return (SYS_CTRL1.getRawData() & static_cast<uint8_t>(BitSYS_CTRL1::TEMP_SEL)) != 0;
}