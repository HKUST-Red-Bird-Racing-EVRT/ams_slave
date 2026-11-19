/**
 * @file main.cpp
 * @author Oscar Chan, Red Bird Racing (oscarckh0818@gmail.com)
 * @brief Main AMS Slave program entry point
 * @version 2.0.0.beta1
 * @date 2026-09-04
 *
 * @copyright Copyright (c) 2026 Red Bird Racing
 *
 */

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
#define I2C_QUEUE_SIZE 8
#define I2C_WATCHDOG_MAX_COUNT 10

AmsState ams;
MCP2515 mcp2515(CS);
I2C<I2C_BITRATE_KBPS, I2C_QUEUE_SIZE, I2C_WATCHDOG_MAX_COUNT> i2c;
Calculator calculator(ams);
CanHelper can_helper(ams, mcp2515, calculator);
BQ76940<I2C_BITRATE_KBPS, I2C_QUEUE_SIZE, I2C_WATCHDOG_MAX_COUNT> bms(ams, i2c, calculator);

uint32_t frame_counter = 0x420;
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

uint8_t last_nack_count = 0;

void loop()
{
	i2c.pump();

	can_frame test_frame = {
		0x67,
		0,
		{0}};

	mcp2515.sendMessage(&test_frame);

	if (i2c.nack_count > last_nack_count)
	{
		last_nack_count = i2c.nack_count;
		ams.fault_flags |= I2C_TIMEOUT_FAULT_BIT;
		can_frame i2c_frame = {
			0x69,
			0,
			{0}};
		mcp2515.sendMessage(&i2c_frame);
	}

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

	// Delete Later
	/*
	can_helper.sendMasterData(0);
	can_helper.sendMasterData(1);
	can_helper.sendMasterData(2);
	can_helper.sendMasterData(3);
	can_helper.sendMasterData(4);
	can_helper.sendMasterData(5);
	can_helper.sendMasterVoltageBuffer();
	*/

	//	Recieve Message from Master
	if (mcp2515.readMessage(&rx_frame) == MCP2515::ERROR_OK)
	{
		//	Recieve Command from Master
		if (rx_frame.can_id == can_helper.MASTER_COMMAND_ADDRESS)
		{
			can_helper.packMasterData(rx_frame);
		}

		//	Recieve Request from Master
		else if (rx_frame.can_id >= can_helper.MASTER_REQUEST_ADDRESS)
		{
			uint8_t message_index = can_helper.getMessageIndexFromAddress(rx_frame.can_id);
			//	Send Voltages
			can_helper.sendMasterData(message_index);
		}
	}

	//	Battery Charging / Idle
	if (ams.is_balancevoltage_recieved && !ams.discharge_active)
	{
		// calculator.setCellBalFlags();
		// bms.writeCellBal();
	}

	//	Check for Faults
	for (uint8_t index = 0; index < NUM_VC; ++index)
	{
		if (calculator.isOverVoltage(ams.cell_voltages[index]))
		{
			ams.fault_flags |= OVERVOLTAGE_FAULT_BIT;
			can_helper.sendPanic(index, ams.cell_voltages[index]);
			break;
		}
		else if (calculator.isUnderVoltage(ams.cell_voltages[index]))
		{
			ams.fault_flags |= UNDERVOLTAGE_FAULT_BIT;
			can_helper.sendPanic(index, ams.cell_voltages[index]);
			break;
		}
	}

	for (uint8_t index = 0; index < NUM_TS; ++index)
	{
		if (calculator.isOverTemperature(ams.temperatures[index]))
		{
			ams.fault_flags |= OVERTEMPERATURE_FAULT_BIT;
			can_helper.sendPanic(index, ams.temperatures[index]);
			break;
		}
		else if (calculator.isUnderTemperature(ams.temperatures[index]))
		{
			ams.fault_flags |= UNDERTEMPERATURE_FAULT_BIT;
			can_helper.sendPanic(index, ams.temperatures[index]);
			break;
		}
	}
}
