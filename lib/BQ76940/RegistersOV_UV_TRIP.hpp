#ifndef REGISTERSOV_UV_TRIP_HPP
#define REGISTERSOV_UV_TRIP_HPP

#include "RegistersClass.hpp"
#include <stdint.h>

extern Register OV_TRIP = Register(RegisterAddress::OV_TRIP, 0xAC);
extern Register UV_TRIP = Register(RegisterAddress::UV_TRIP, 0x97);

void loadOVTrip();
void loadUVTrip();

#endif  // REGISTERSOV_UV_TRIP_HPP