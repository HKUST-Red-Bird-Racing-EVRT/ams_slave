#ifndef TEMPERATURE_HPP
#define TEMPERATURE_HPP

#include "AmsState.hpp"
#include <stdint.h>

constexpr uint16_t VOLTAGE_OT = 5000; // millidegrees Celsius (millivolts)
constexpr uint16_t VOLTAGE_UT = 0;    // millidegrees Celsius (millivolts)

void checkAndSetTemperatureFaults(AmsState &state);

#endif  //  TEMPERATURE_HPP