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

AmsState ams;
MCP2515 mcp2515(CS);
// I2C<100, 4, 4, 10> i2c;
I2C<I2C_BITRATE_KBPS, I2C_PRIORITY_SIZE, I2C_RECURRING_SIZE, I2C_WATCHDOG_MAX_COUNT> i2c;
Calculator calculator(ams);
CanHelper can_helper(ams, mcp2515);
BQ76940<I2C_BITRATE_KBPS, I2C_PRIORITY_SIZE, I2C_RECURRING_SIZE, I2C_WATCHDOG_MAX_COUNT> bms(ams, i2c, calculator);

uint32_t frame_counter = 0x420;
can_frame test_frame = {frame_counter++, 1, {0X69}};
can_frame rx_frame;

ISR(TWI_vect)
{
	i2c.handleIsr();
}

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

	mcp2515.sendMessage(&test_frame); // 420
	test_frame = {frame_counter++, 1, {0X69}};

	//	Set up Slave ID
	bool jp1 = digitalRead(JP1);
	bool jp2 = digitalRead(JP2);
	bool jp3 = digitalRead(JP3);
	bool jp4 = digitalRead(JP4);

	mcp2515.sendMessage(&test_frame); // 421
	test_frame = {frame_counter++, 1, {0X69}};

	can_helper.setNodeID(jp1, jp2, jp3, jp4);

	// 423
	mcp2515.sendMessage(&test_frame); // 422
	test_frame = {frame_counter++, 1, {0X69}};

	//	Read Data from Register ADCGAIN1, ADCGAIN2, ADCOFFSET
	uint8_t adc_gain1_data = bms.getRegisterReadData(REGISTER_ADCGAIN1_ADDRESS);
	uint8_t adc_gain2_data = bms.getRegisterReadData(REGISTER_ADCGAIN2_ADDRESS);
	uint8_t adc_offset_data = bms.getRegisterReadData(REGISTER_ADCOFFSET_ADDRESS);

	mcp2515.sendMessage(&test_frame); // 423
	test_frame = {frame_counter++, 1, {0X69}};

	calculator.setAdcGain(adc_gain1_data, adc_gain2_data);
	calculator.setAdcOffset(adc_offset_data);
}

void loop()
{
	i2c.pump();
	
	//	Read Data from Register SYS_CTRL2
	uint8_t sys_ctrl2_data = bms.getRegisterReadData(REGISTER_SYS_CTRL2_ADDRESS);
	calculator.setDischargingState(sys_ctrl2_data);

	bms.readVoltage(); // updates timestamp if had read
	// calculator.setVoltageMin();

	// ntc.readTemperature();
	ams.temperatures[0] = analogRead(NTC1);
	ams.temperatures[1] = analogRead(NTC2);
	ams.temperatures[2] = analogRead(NTC3);
	ams.temperatures[3] = analogRead(NTC4);
	ams.temperatures[4] = analogRead(NTC5);

	//	Battery Charging / Idle
	if (ams.cellbal_active)
	{
		ams.cellbal_flags &= ~DISCHARGE_STATE_BIT;
		ams.cellbal_flags |= CELLBAL_STATE_BIT;

		if (ams.is_minvlotage_recieved)
		{
			calculator.setCellBalFlags();
		}
	}

	//	Battery Discharging
	if (ams.discharge_active)
	{
		ams.cellbal_flags = DISCHARGE_STATE_BIT;
		ams.is_minvlotage_recieved = false;

		for (uint8_t index = 0; index < NUM_VC; ++index)
		{
			if (calculator.isOverVoltage(ams.cell_voltages[index]))
			{
				ams.fault_flags |= OVERVOLTAGE_FAULT_BIT;
				can_helper.sendPanic();
				break;
			}
			else if (calculator.isUnderVoltage(ams.cell_voltages[index]))
			{
				ams.fault_flags |= UNDERVOLTAGE_FAULT_BIT;
				can_helper.sendPanic();
				break;
			}
		}

		for (uint8_t index = 0; index < NUM_TS; ++index)
		{
			if (calculator.isOverTemperature(ams.temperatures[index]))
			{
				ams.fault_flags |= OVERTEMPERATURE_FAULT_BIT;
				can_helper.sendPanic();
				break;
			}
			else if (calculator.isUnderTemperature(ams.temperatures[index]))
			{
				ams.fault_flags |= UNDERTEMPERATURE_FAULT_BIT;
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

	//	Recieve Message from Master
	if (mcp2515.readMessage(&rx_frame) == MCP2515::ERROR_OK)
	{
		if (rx_frame.can_id == can_helper.MASTER_ADDRESS)
		{
			can_helper.sendVoltages(0); // later modify
		}
	}
}
