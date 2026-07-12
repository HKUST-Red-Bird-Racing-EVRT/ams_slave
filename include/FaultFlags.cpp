#include "FaultFlags.hpp"

bool FaultFlags::getOverallFault() const
{
    return flags & OVERALL;
}

void FaultFlags::setOverallFault()
{
    flags |= OVERALL;
}

void FaultFlags::clearOverallFault()
{
    flags &= ~OVERALL;
}



bool FaultFlags::getOvervoltageFault() const { 
    return flags & OVERVOLTAGE; 
}

void FaultFlags::setOvervoltageFault() { 
    flags |= OVERVOLTAGE;
    setOverallFault(); 
}

void FaultFlags::clearOvervoltageFault() { 
    flags &= ~OVERVOLTAGE; 
}



bool FaultFlags::getUndervoltageFault() const { 
    return flags & UNDERVOLTAGE; 
}
void FaultFlags::setUndervoltageFault() { 
    flags |= UNDERVOLTAGE;
    setOverallFault(); 
}
void FaultFlags::clearUndervoltageFault() { 
    flags &= ~UNDERVOLTAGE; 
}



bool FaultFlags::getOvertemperatureFault() const { 
    return flags & OVERTEMPERATURE; 
}
void FaultFlags::setOvertemperatureFault() { 
    flags |= OVERTEMPERATURE; 
    setOverallFault();
}
void FaultFlags::clearOvertemperatureFault() { 
    flags &= ~OVERTEMPERATURE; 
}



bool FaultFlags::getUndertemperatureFault() const { 
     return flags & UNDERTEMPERATURE; 
}

void FaultFlags::setUndertemperatureFault() { 
    flags |= UNDERTEMPERATURE; 
    setOverallFault();
}
void FaultFlags::clearUndertemperatureFault() { 
    flags &= ~UNDERTEMPERATURE; 
}



bool FaultFlags::getI2CTimeoutFault() const { 
    return flags & I2C_TIMEOUT; 
}
void FaultFlags::setI2CTimeoutFault() { 
    flags |= I2C_TIMEOUT; 
    setOverallFault();
}
void FaultFlags::clearI2CTimeoutFault() { 
    flags &= ~I2C_TIMEOUT; 
}