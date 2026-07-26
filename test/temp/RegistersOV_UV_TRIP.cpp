#include "RegistersOV_UV_TRIP.hpp"
#include "Voltage.hpp"

void loadOVTrip()
{
    OV_TRIP.load(OV_TRIP_THRESHOLD);
}

void loadUVTrip()
{
    UV_TRIP.load(UV_TRIP_THRESHOLD);
}