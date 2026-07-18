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
I2C<100, 4, 4, 10> i2c;
uint8_t adc_gain, adc_offset;

void setup()
{
	mcp2515.reset();
	mcp2515.setBitrate(CAN_500KBPS, MCP_20MHZ);
	mcp2515.setNormalMode();

	pinMode(NTC1, INPUT);
	pinMode(NTC2, INPUT);
	pinMode(NTC3, INPUT);
	pinMode(NTC4, INPUT);
	pinMode(NTC5, INPUT);

	uint8_t adc_gain1_buffer[2] = {0};
	uint8_t adc_offset_buffer[2] = {0};
	uint8_t adc_gain2_buffer[2] = {0};

	const uint8_t send_adc[1]{static_cast<const uint8_t>(RegisterAddress::ADCGAIN1)};
	const I2cTransaction send_adc_gain1_action = I2cTransaction::makeChainedWrite(IC_ADDRESS, 1, send_adc);

	while (!i2c.pushRecurring(send_adc_gain1_action))
		;

	while (!i2c.pushRecurring(I2cTransaction::makeRead(IC_ADDRESS, 2, adc_gain1_buffer)))
		;
	i2c.pump();
}

BQ76940<100, 4, 4, 10> bms(ams_state, i2c, adc_gain, adc_offset);

void loop()
{
	i2c.pump();
	bms.readVoltage(); // updates timestamp if had read

	// ams_state.checkVoltage(); // checks for overvoltage and undervoltage faults

	// ntc.readTemperature();
	ams_state.temperatures[0] = analogRead(NTC1);
	ams_state.temperatures[1] = analogRead(NTC2);
	ams_state.temperatures[2] = analogRead(NTC3);
	ams_state.temperatures[3] = analogRead(NTC4);
	ams_state.temperatures[4] = analogRead(NTC5);

	// ams_state.checkVoltage();
	// ams_state.checkTemperature(); // checks for overtemperature and undertemperature faults

	// if (ams_state.fault_flags.getOverallFault())
	// {
	// 	// Handle fault condition
	// 	mcp2515.sendFrame(ams_state.getFaultFrame());
	// }
}
