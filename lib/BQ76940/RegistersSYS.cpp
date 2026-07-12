#include "RegistersSYS.hpp"

//  Register SYS_STAT Functions

//  CCReady

void SYS_STAT_setCCReady(bool value)
{
    if (value)
    {
        SYS_STAT.load(SYS_STAT.getRawData() | static_cast<uint8_t>(BitSYS_STAT::CC_READY));
    }
    else
    {
        SYS_STAT.load(SYS_STAT.getRawData() & ~static_cast<uint8_t>(BitSYS_STAT::CC_READY));
    }
}

bool SYS_STAT_getCCReady()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::CC_READY)) != 0;
}

//  Internal Device Fault

void SYS_STAT_setInternalDeviceFault(bool value)
{
    if (value)
    {
        SYS_STAT.load(SYS_STAT.getRawData() | static_cast<uint8_t>(BitSYS_STAT::DEVICE_XREADY));
    }
    else
    {
        SYS_STAT.load(SYS_STAT.getRawData() & ~static_cast<uint8_t>(BitSYS_STAT::DEVICE_XREADY));
    }
}

bool SYS_STAT_getInternalDeviceFault()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::DEVICE_XREADY)) != 0;
}

//  External Override Alert

void SYS_STAT_setExternalOverrideAlert(bool value)
{
    if (value)
    {
        SYS_STAT.load(SYS_STAT.getRawData() | static_cast<uint8_t>(BitSYS_STAT::OVRD_ALERT));
    }
    else
    {
        SYS_STAT.load(SYS_STAT.getRawData() & ~static_cast<uint8_t>(BitSYS_STAT::OVRD_ALERT));
    }
}

bool SYS_STAT_getExternalOverrideAlert()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::OVRD_ALERT)) != 0;
}

//  Undervoltage Fault

void SYS_STAT_setUndervoltageFault(bool value)
{
    if (value)
    {
        SYS_STAT.load(SYS_STAT.getRawData() | static_cast<uint8_t>(BitSYS_STAT::UV));
    }
    else
    {
        SYS_STAT.load(SYS_STAT.getRawData() & ~static_cast<uint8_t>(BitSYS_STAT::UV));
    }
}

bool SYS_STAT_getUndervoltageFault()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::UV)) != 0;
}

//  Overvoltage Fault

void SYS_STAT_setOvervoltageFault(bool value)
{
    if (value)
    {
        SYS_STAT.load(SYS_STAT.getRawData() | static_cast<uint8_t>(BitSYS_STAT::OV));
    }
    else
    {
        SYS_STAT.load(SYS_STAT.getRawData() & ~static_cast<uint8_t>(BitSYS_STAT::OV));
    }
}

bool SYS_STAT_getOvervoltageFault()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::OV)) != 0;
}

//  Short Circuit Fault

void SYS_STAT_setShortCircuitFault(bool value)
{
    if (value)
    {
        SYS_STAT.load(SYS_STAT.getRawData() | static_cast<uint8_t>(BitSYS_STAT::SCD));
    }
    else
    {
        SYS_STAT.load(SYS_STAT.getRawData() & ~static_cast<uint8_t>(BitSYS_STAT::SCD));
    }
}

bool SYS_STAT_getShortCircuitFault()
{
    return (SYS_STAT.getRawData() & static_cast<uint8_t>(BitSYS_STAT::SCD)) != 0;
}

//  Overcurrent Fault

void SYS_STAT_setOvercurrentFault(bool value)
{
    if (value)
    {
        SYS_STAT.load(SYS_STAT.getRawData() | static_cast<uint8_t>(BitSYS_STAT::OCD));
    }
    else
    {
        SYS_STAT.load(SYS_STAT.getRawData() & ~static_cast<uint8_t>(BitSYS_STAT::OCD));
    }
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

bool SYS_CTRL1_getShipModeEnabled()
{
    if ((SYS_CTRL1.getRawData() & static_cast<uint8_t>(BitSYS_CTRL1::SHUT_A)) != 0 &&
           (SYS_CTRL1.getRawData() & static_cast<uint8_t>(BitSYS_CTRL1::SHUT_B)) == 0)
    {
        return true;
    }

    if ((SYS_CTRL1.getRawData() & static_cast<uint8_t>(BitSYS_CTRL1::SHUT_A)) == 0 &&
           (SYS_CTRL1.getRawData() & static_cast<uint8_t>(BitSYS_CTRL1::SHUT_B)) != 0)
    {
        return true;
    }

    return false;
}