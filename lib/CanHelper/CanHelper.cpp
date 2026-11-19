/**
 * @file CanHelper.cpp
 * @brief Implementation of AMS slave CAN communication.
 */

#include "CanHelper.hpp"

CanHelper::CanHelper(AmsState &ams_, MCP2515 &mcp2515_, Calculator &calculator_)
    : ams(ams_),
      mcp2515(mcp2515_),
      calculator(calculator_)
{
}

void CanHelper::setNodeID(bool &jp1, bool &jp2, bool &jp3, bool &jp4)
{
    uint8_t id = 0;
    id |= (jp1 << 0);
    id |= (jp2 << 1);
    id |= (jp3 << 2);
    id |= (jp4 << 3);
    ams.node_id = id;
    PANIC_MESSAGE_ADDRESS += id;
    MASTER_COMMAND_ADDRESS += id;
    SLAVE_MESSAGE_ADDRESS += id;
    SLAVE_VOLTAGE_BUFFER_ADDRESS += id;
}

uint16_t CanHelper::getSlaveMessageAddress(uint8_t index)
{
    return SLAVE_MESSAGE_ADDRESS + index * 0x10;
}

uint16_t CanHelper::getSlaveVoltageBufferAddress(uint8_t index)
{
    return SLAVE_VOLTAGE_BUFFER_ADDRESS + index * 0x10;
}

uint8_t CanHelper::getMessageIndexFromAddress(uint16_t address)
{
    return address - MASTER_REQUEST_ADDRESS;
}

void CanHelper::sendMasterData(uint8_t index)
{
    switch (index)
    {
    //  Send Frame 0
    case 0x00:
    {
        can_frame send_voltage_frame_0 = {
            getSlaveMessageAddress(0),
            3,
            ams.command_flags,
            ams.cellbal_flags & 0xFF,
            (ams.cellbal_flags >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_0);
        break;
    }

    //  Send Frame 1
    case 0x01:
    {
        can_frame send_voltage_frame_1 = {
            getSlaveMessageAddress(1),
            8,
            ams.cell_voltages[0] & 0xFF,
            (ams.cell_voltages[0] >> 8) & 0xFF,
            ams.cell_voltages[1] & 0xFF,
            (ams.cell_voltages[1] >> 8) & 0xFF,
            ams.cell_voltages[2] & 0xFF,
            (ams.cell_voltages[2] >> 8) & 0xFF,
            ams.cell_voltages[3] & 0xFF,
            (ams.cell_voltages[3] >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_1);
        break;
    }

    //  Send Frame 2
    case 0x02:
    {
        can_frame send_voltage_frame_2 = {
            getSlaveMessageAddress(2),
            8,
            ams.cell_voltages[4] & 0xFF,
            (ams.cell_voltages[4] >> 8) & 0xFF,
            ams.cell_voltages[5] & 0xFF,
            (ams.cell_voltages[5] >> 8) & 0xFF,
            ams.cell_voltages[6] & 0xFF,
            (ams.cell_voltages[6] >> 8) & 0xFF,
            ams.cell_voltages[7] & 0xFF,
            (ams.cell_voltages[7] >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_2);
        break;
    }

    //  Send Frame 3
    case 0x03:
    {
        can_frame send_voltage_frame_3 = {
            getSlaveMessageAddress(3),
            8,
            ams.cell_voltages[8] & 0xFF,
            (ams.cell_voltages[8] >> 8) & 0xFF,
            ams.cell_voltages[9] & 0xFF,
            (ams.cell_voltages[9] >> 8) & 0xFF,
            ams.cell_voltages[10] & 0xFF,
            (ams.cell_voltages[10] >> 8) & 0xFF,
            ams.cell_voltages[11] & 0xFF,
            (ams.cell_voltages[11] >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_3);
        break;
    }

    //  Send Frame 4
    case 0x04:
    {
        can_frame send_voltage_frame_4 = {
            getSlaveMessageAddress(4),
            8,
            ams.cell_voltages[12] & 0xFF,
            (ams.cell_voltages[12] >> 8) & 0xFF,
            ams.cell_voltages[13] & 0xFF,
            (ams.cell_voltages[13] >> 8) & 0xFF,
            ams.temperatures[0] & 0xFF,
            (ams.temperatures[0] >> 8) & 0xFF,
            ams.temperatures[1] & 0xFF,
            (ams.temperatures[1] >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_4);
        break;
    }

    //  Send Frame 5
    case 0x05:
    {
        can_frame send_voltage_frame_5 = {
            getSlaveMessageAddress(5),
            6,
            ams.temperatures[2] & 0xFF,
            (ams.temperatures[2] >> 8) & 0xFF,
            ams.temperatures[3] & 0xFF,
            (ams.temperatures[3] >> 8) & 0xFF,
            ams.temperatures[4] & 0xFF,
            (ams.temperatures[4] >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_5);
        break;
    }

    default:
        break;
    }
}

void CanHelper::sendPanic(uint8_t index, uint16_t data)
{
    can_frame send_frame = {
        PANIC_MESSAGE_ADDRESS, 
        4, 
        ams.fault_flags,
        index,
        data & 0xFF,
        (data >> 8) & 0xFF
    };

    mcp2515.sendMessage(&send_frame);
}

void CanHelper::packMasterData(can_frame &frame)
{
    uint8_t command_byte = frame.data[0];

    ams.discharge_active = command_byte & MASTERCMD_DISCHARGE_STATE_BIT;
    ams.cellbal_active = command_byte & MASTERCMD_CELLBAL_STATE_BIT;
    ams.cellbal_odd = command_byte & MASTERCMD_CELLBAL_ODD_BIT;

    if (ams.cellbal_active)
    {
        ams.command_flags &= ~DISCHARGE_STATE_BIT;
        ams.command_flags |= CELLBAL_STATE_BIT;
        ams.is_balancevoltage_recieved = true;
        calculator.VOLTAGE_BALANCE = frame.data[1] | (frame.data[2] << 8);
        return;
    }

    if (ams.discharge_active)
    {
        ams.command_flags = DISCHARGE_STATE_BIT;
        ams.is_balancevoltage_recieved = false;
        calculator.VOLTAGE_BALANCE = 0;
        return;
    }

    calculator.VOLTAGE_BALANCE = 0;
}

void CanHelper::sendMasterVoltageBuffer()
{
    can_frame send_voltage_buffer_frame_0 = {
        getSlaveVoltageBufferAddress(0),
        8,
        ams.cell_voltages_buffer[0] & 0xFF,
        ams.cell_voltages_buffer[1] & 0xFF,
        ams.cell_voltages_buffer[2] & 0xFF,
        ams.cell_voltages_buffer[3] & 0xFF,
        ams.cell_voltages_buffer[4] & 0xFF,
        ams.cell_voltages_buffer[5] & 0xFF,
        ams.cell_voltages_buffer[6] & 0xFF,
        ams.cell_voltages_buffer[7] & 0xFF,
    };

    mcp2515.sendMessage(&send_voltage_buffer_frame_0);

    can_frame send_voltage_buffer_frame_1 = {
        getSlaveVoltageBufferAddress(1),
        8,
        ams.cell_voltages_buffer[8] & 0xFF,
        ams.cell_voltages_buffer[9] & 0xFF,
        ams.cell_voltages_buffer[10] & 0xFF,
        ams.cell_voltages_buffer[11] & 0xFF,
        ams.cell_voltages_buffer[12] & 0xFF,
        ams.cell_voltages_buffer[13] & 0xFF,
        ams.cell_voltages_buffer[14] & 0xFF,
        ams.cell_voltages_buffer[15] & 0xFF,
    };

    mcp2515.sendMessage(&send_voltage_buffer_frame_1);

    can_frame send_voltage_buffer_frame_2 = {
        getSlaveVoltageBufferAddress(2),
        8,
        ams.cell_voltages_buffer[16] & 0xFF,
        ams.cell_voltages_buffer[17] & 0xFF,
        ams.cell_voltages_buffer[18] & 0xFF,
        ams.cell_voltages_buffer[19] & 0xFF,
        ams.cell_voltages_buffer[20] & 0xFF,
        ams.cell_voltages_buffer[21] & 0xFF,
        ams.cell_voltages_buffer[22] & 0xFF,
        ams.cell_voltages_buffer[23] & 0xFF,
    };

    mcp2515.sendMessage(&send_voltage_buffer_frame_2);

    can_frame send_voltage_buffer_frame_3 = {
        getSlaveVoltageBufferAddress(3),
        8,
        ams.cell_voltages_buffer[24] & 0xFF,
        ams.cell_voltages_buffer[25] & 0xFF,
        ams.cell_voltages_buffer[26] & 0xFF,
        ams.cell_voltages_buffer[27] & 0xFF,
        ams.cell_voltages_buffer[28] & 0xFF,
        ams.cell_voltages_buffer[29] & 0xFF,
        (calculator.ADC_GAIN - 365) & 0xFF,
        calculator.ADC_OFFSET & 0xFF,
    };

    mcp2515.sendMessage(&send_voltage_buffer_frame_3);
}