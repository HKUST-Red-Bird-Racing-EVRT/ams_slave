#include <Arduino.h>
#include "BoardConfig.h"

#include "BQ76940.hpp"
#include "I2C.hpp"
#include "AmsState.hpp"

// ignore -Wpedantic warnings for mcp2515.h
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <mcp2515.h>
#pragma GCC diagnostic pop

AmsState ams_state;

MCP2515 mcp2515(CS);
I2C <100, 4, 4, 10> i2c;
BQ76940 bms(ams_state);

void setup()
{

}

void loop()
{
	i2c.pump();
	bms.readVoltage(); // updates timestamp if had read

	// ntc.readTemperature();
	ams_state.temperatures[0] = analogRead(NTC1);
	ams_state.temperatures[1] = analogRead(NTC2);
	ams_state.temperatures[2] = analogRead(NTC3);
	ams_state.temperatures[3] = analogRead(NTC4);
	ams_state.temperatures[4] = analogRead(NTC5);

	control.check(); // also checks if now - timestamp > threshold -> fault
	if(control.hasFault())
	{
		// Handle fault condition
		can.sendFrame(control.getFaultFrame());
	}
}
