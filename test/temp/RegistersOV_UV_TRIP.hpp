#ifndef REGISTERSOV_UV_TRIP_HPP
#define REGISTERSOV_UV_TRIP_HPP

#include "RegistersClass.hpp"
#include <stdint.h>

inline Register OV_TRIP = Register(RegisterAddress::OV_TRIP, 0xAC);
inline Register UV_TRIP = Register(RegisterAddress::UV_TRIP, 0x97);

void loadOVTrip();
void loadUVTrip();

#endif  // REGISTERSOV_UV_TRIP_HPP