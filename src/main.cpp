#include <Arduino.h>
#include "BoardConfig.h"
#include "BqRegsiters.h"

#include "AmsState.hpp"

#include "BQ76940.hpp"
#include "I2C.hpp"
#include "Calculator.hpp"
#include "CanHelper.hpp"

// ignore -Wpedantic warnings for mcp2515.h
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <mcp2515.h>
#pragma GCC diagnostic pop

#define I2C_BITRATE_KBPS 100
#define I2C_PRIORITY_SIZE 4
#define I2C_RECURRING_SIZE 4
#define I2C_WATCHDOG_MAX_COUNT 10

AmsState ams_state;

MCP2515 mcp2515(CS);
// I2C<100, 4, 4, 10> i2c;
I2C<I2C_BITRATE_KBPS, I2C_PRIORITY_SIZE, I2C_RECURRING_SIZE, I2C_WATCHDOG_MAX_COUNT> i2c;
Calculator calculator(ams_state);
CanHelper can_helper(ams_state);

void setup()
{
	mcp2515.reset();
	mcp2515.setBitrate(CAN_500KBPS, MCP_20MHZ);
	mcp2515.setNormalMode();

	pinMode(ALERT, INPUT);
	pinMode(CS, OUTPUT);

	pinMode(NTC1, INPUT);
	pinMode(NTC2, INPUT);
	pinMode(NTC3, INPUT);
	pinMode(NTC4, INPUT);
	pinMode(NTC5, INPUT);

	pinMode(JP1, INPUT);
	pinMode(JP2, INPUT);
	pinMode(JP3, INPUT);
	pinMode(JP4, INPUT);

	//	Set up Slave ID
	bool jp1 = digitalRead(JP1);
	bool jp2 = digitalRead(JP2);
	bool jp3 = digitalRead(JP3);
	bool jp4 = digitalRead(JP4);
	can_helper.setNodeID(jp1, jp2, jp3, jp4);

	//	Read data from Register ADCGAIN1
	uint8_t adc_gain1_buffer[2] = {0};

	const uint8_t send_adc_gain1[1] = {REGISTER_ADCGAIN1_ADDRESS};

	while (!i2c.pushPriority(I2cTransaction::makeChainedWrite(IC_ADDRESS, 1, send_adc_gain1)))
		;

	while (!i2c.pushPriority(I2cTransaction::makeRead(IC_ADDRESS, 2, adc_gain1_buffer)))
		;
	i2c.pump();

	//	Read Data from Register ADCGAIN2
	uint8_t adc_gain2_buffer[2] = {0};

	const uint8_t send_adc_gain2[1] = {REGISTER_ADCGAIN2_ADDRESS};

	while (!i2c.pushRecurring(I2cTransaction::makeChainedWrite(IC_ADDRESS, 1, send_adc_gain2)))
		;

	while (!i2c.pushRecurring(I2cTransaction::makeRead(IC_ADDRESS, 2, adc_gain2_buffer)))
		;
	i2c.pump();

	//	Read Data from Register ADC_OFFSET
	uint8_t adc_offset_buffer[2] = {0};

	const uint8_t send_adc_offset[1] = {REGISTER_ADCOFFSET_ADDRESS};

	while (!i2c.pushRecurring(I2cTransaction::makeChainedWrite(IC_ADDRESS, 1, send_adc_offset)))
		;

	while (!i2c.pushRecurring(I2cTransaction::makeRead(IC_ADDRESS, 2, adc_offset_buffer)))
		;
	i2c.pump();

	//	Read Data from Register PROTECT3
	uint8_t protect3_buffer[2] = {0};

	const uint8_t send_protect3[1] = {REGISTER_PROTECT3_ADDRESS};

	while (!i2c.pushRecurring(I2cTransaction::makeChainedWrite(IC_ADDRESS, 1, send_protect3)))
		;

	while (!i2c.pushRecurring(I2cTransaction::makeRead(IC_ADDRESS, 2, protect3_buffer)))
		;
	i2c.pump();

	calculator.setAdcGain(adc_gain1_buffer[1], adc_gain2_buffer[1]);
	calculator.setAdcOffset(adc_offset_buffer[1]);
}

BQ76940<I2C_BITRATE_KBPS, I2C_PRIORITY_SIZE, I2C_RECURRING_SIZE, I2C_WATCHDOG_MAX_COUNT> bms(ams_state, i2c, calculator);

void loop()
{
	i2c.pump();
	bms.readVoltage(); // updates timestamp if had read
	calculator.setVoltageMin();

	can_frame send_voltage_frame = {
		can_helper.SLAVE_ADDRESS,
		8,
		ams_state.cell_voltages[0] & 0xFF,
		(ams_state.cell_voltages[0] >> 8) & 0xFF,
		ams_state.cell_voltages[1] & 0xFF,
		(ams_state.cell_voltages[1] >> 8) & 0xFF,
		ams_state.cell_voltages[2] & 0xFF,
		(ams_state.cell_voltages[2] >> 8) & 0xFF,
		ams_state.cell_voltages[3] & 0xFF,
		(ams_state.cell_voltages[3] >> 8) & 0xFF,
	};

	mcp2515.sendMessage(&send_voltage_frame);

	//	Read Data from Register SYS_CTRL2
	uint8_t sys_ctrl2_buffer[2] = {0};

	const uint8_t send_sys_ctrl2[1] = {REGISTER_SYS_CTRL2_ADDRESS};

	while (!i2c.pushRecurring(I2cTransaction::makeChainedWrite(IC_ADDRESS, 1, send_sys_ctrl2)))
		;

	while (!i2c.pushRecurring(I2cTransaction::makeRead(IC_ADDRESS, 2, sys_ctrl2_buffer)))
		;
	i2c.pump();

	//	Battery Charging / Idle
	if (calculator.isCellBalActivated(sys_ctrl2_buffer[1]))
	{
		/*

		*/
	}

	//	Battery Discharging
	else
	{
		for (uint8_t index = 0; index < NUM_VC; ++index)
		{
			if (calculator.isOverVoltage(ams_state.cell_voltages[index]))
			{
				ams_state.fault_flags.setOvervoltageFault();
				const can_frame send_frame = {can_helper.PANIC_ADDRESS, 1, ams_state.fault_flags.getFlags()};
				mcp2515.sendMessage(&send_frame);
				break;
			}
			else if (calculator.isUnderVoltage(ams_state.cell_voltages[index]))
			{
				ams_state.fault_flags.setUndervoltageFault();
				const can_frame send_frame = {can_helper.PANIC_ADDRESS, 1, ams_state.fault_flags.getFlags()};
				mcp2515.sendMessage(&send_frame);
				break;
			}
		}

		// ntc.readTemperature();
		ams_state.temperatures[0] = analogRead(NTC1);
		ams_state.temperatures[1] = analogRead(NTC2);
		ams_state.temperatures[2] = analogRead(NTC3);
		ams_state.temperatures[3] = analogRead(NTC4);
		ams_state.temperatures[4] = analogRead(NTC5);

		for (uint8_t index = 0; index < NUM_TS; ++index)
		{
			if (calculator.isOverTemperature(ams_state.temperatures[index]))
			{
				ams_state.fault_flags.setOvertemperatureFault();
				const can_frame send_frame = {can_helper.PANIC_ADDRESS, 1, ams_state.fault_flags.getFlags()};
				mcp2515.sendMessage(&send_frame);
				break;
			}
			else if (calculator.isUnderTemperature(ams_state.temperatures[index]))
			{
				ams_state.fault_flags.setUndertemperatureFault();
				const can_frame send_frame = {can_helper.PANIC_ADDRESS, 1, ams_state.fault_flags.getFlags()};
				mcp2515.sendMessage(&send_frame);
				break;
			}
		}
	}
}
