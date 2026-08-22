#include "CanHelper.hpp"

CanHelper::CanHelper(AmsState &ams_state_, MCP2515 &mcp2515_)
    : ams_state(ams_state_),
      mcp2515(mcp2515_)
{
}

void CanHelper::setNodeID(bool &jp1, bool &jp2, bool &jp3, bool &jp4)
{
    uint8_t id = 0;
    id |= (jp1 << 0);
    id |= (jp2 << 1);
    id |= (jp3 << 2);
    id |= (jp4 << 3);
    ams_state.node_id = id;
    PANIC_ADDRESS += id;
    MASTER_ADDRESS += id;
    SLAVE_ADDRESS += id;
}

uint16_t CanHelper::getSlaveAddress(uint8_t index)
{
    return SLAVE_ADDRESS + index * NUM_SLAVE;
}

void CanHelper::sendVoltages(uint8_t index)
{
    switch (index)
    {
    //  Send Frame 0
    case 0:
        can_frame send_voltage_frame_0 = {
            getSlaveAddress(0),
            8,
            ams_state.cellbal_flags & 0xFF,
            (ams_state.cellbal_flags >> 8) & 0xFF,
            ams_state.cell_voltages[0] & 0xFF,
            (ams_state.cell_voltages[0] >> 8) & 0xFF,
            ams_state.cell_voltages[1] & 0xFF,
            (ams_state.cell_voltages[1] >> 8) & 0xFF,
            ams_state.cell_voltages[2] & 0xFF,
            (ams_state.cell_voltages[2] >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_0);
        break;

    //  Send Frame 1
    case 1:
        can_frame send_voltage_frame_1 = {
            getSlaveAddress(1),
            8,
            ams_state.cell_voltages[3] & 0xFF,
            (ams_state.cell_voltages[3] >> 8) & 0xFF,
            ams_state.cell_voltages[4] & 0xFF,
            (ams_state.cell_voltages[4] >> 8) & 0xFF,
            ams_state.cell_voltages[5] & 0xFF,
            (ams_state.cell_voltages[5] >> 8) & 0xFF,
            ams_state.cell_voltages[6] & 0xFF,
            (ams_state.cell_voltages[6] >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_1);
        break;

    //  Send Frame 2
    case 2:
        can_frame send_voltage_frame_2 = {
            getSlaveAddress(2),
            8,
            ams_state.cell_voltages[7] & 0xFF,
            (ams_state.cell_voltages[7] >> 8) & 0xFF,
            ams_state.cell_voltages[8] & 0xFF,
            (ams_state.cell_voltages[8] >> 8) & 0xFF,
            ams_state.cell_voltages[9] & 0xFF,
            (ams_state.cell_voltages[9] >> 8) & 0xFF,
            ams_state.cell_voltages[10] & 0xFF,
            (ams_state.cell_voltages[10] >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_2);
        break;

    //  Send Frame 3
    case 3:
        can_frame send_voltage_frame_3 = {
            getSlaveAddress(3),
            8,
            ams_state.cell_voltages[11] & 0xFF,
            (ams_state.cell_voltages[11] >> 8) & 0xFF,
            ams_state.cell_voltages[12] & 0xFF,
            (ams_state.cell_voltages[12] >> 8) & 0xFF,
            ams_state.cell_voltages[13] & 0xFF,
            (ams_state.cell_voltages[13] >> 8) & 0xFF,
            ams_state.temperatures[0] & 0xFF,
            (ams_state.temperatures[0] >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_3);
        break;

    //  Send Frame 4
    case 4:
        can_frame send_voltage_frame_4 = {
            getSlaveAddress(4),
            8,
            ams_state.temperatures[1] & 0xFF,
            (ams_state.temperatures[1] >> 8) & 0xFF,
            ams_state.temperatures[2] & 0xFF,
            (ams_state.temperatures[2] >> 8) & 0xFF,
            ams_state.temperatures[3] & 0xFF,
            (ams_state.temperatures[3] >> 8) & 0xFF,
            ams_state.temperatures[4] & 0xFF,
            (ams_state.temperatures[4] >> 8) & 0xFF,
        };

        mcp2515.sendMessage(&send_voltage_frame_4);
        break;

    default:
        break;
    }
}

void CanHelper::sendPanic()
{
    can_frame send_frame = {PANIC_ADDRESS, 1, ams_state.fault_flags.getFlags()};
	mcp2515.sendMessage(&send_frame);
}