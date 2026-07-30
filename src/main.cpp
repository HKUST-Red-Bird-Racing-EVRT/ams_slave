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
CanHelper can_helper(ams_state, mcp2515);
BQ76940<I2C_BITRATE_KBPS, I2C_PRIORITY_SIZE, I2C_RECURRING_SIZE, I2C_WATCHDOG_MAX_COUNT> bms(ams_state, i2c, calculator);

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

	//	Read Data from Register ADCGAIN1, ADCGAIN2, ADCOFFSET
	uint8_t adc_gain1_data = bms.getRegisterReadData(REGISTER_ADCGAIN1_ADDRESS);
	uint8_t adc_gain2_data = bms.getRegisterReadData(REGISTER_ADCGAIN2_ADDRESS);
	uint8_t adc_offset_data = bms.getRegisterReadData(REGISTER_ADCOFFSET_ADDRESS);

	calculator.setAdcGain(adc_gain1_data, adc_gain2_data);
	calculator.setAdcOffset(adc_offset_data);
}

void loop()
{
	i2c.pump();
	bms.readVoltage(); // updates timestamp if had read
	calculator.setVoltageMin();

	//	Read Data from Register SYS_CTRL2
	uint8_t sys_ctrl2_data = bms.getRegisterReadData(REGISTER_SYS_CTRL2_ADDRESS);

	//	Battery Charging / Idle
	if (calculator.isCellBalActivated(sys_ctrl2_data))
	{
		calculator.setCellBalFlags();
	}

	//	Battery Discharging
	else
	{
		ams_state.cellbal_flags = 0x00;

		for (uint8_t index = 0; index < NUM_VC; ++index)
		{
			if (calculator.isOverVoltage(ams_state.cell_voltages[index]))
			{
				ams_state.fault_flags.setOvervoltageFault();
				can_helper.sendPanic();
				break;
			}
			else if (calculator.isUnderVoltage(ams_state.cell_voltages[index]))
			{
				ams_state.fault_flags.setUndervoltageFault();
				can_helper.sendPanic();
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
				can_helper.sendPanic();
				break;
			}
			else if (calculator.isUnderTemperature(ams_state.temperatures[index]))
			{
				ams_state.fault_flags.setUndertemperatureFault();
				can_helper.sendPanic();
				break;
			}
		}
	}

	//	Send Voltages
	for (uint8_t index = 0; index < NUM_SLAVE_FRAME; ++index)
	{
		can_helper.sendVoltages(index);
	}
}
