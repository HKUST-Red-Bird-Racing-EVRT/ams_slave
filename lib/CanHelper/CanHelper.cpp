#include "CanHelper.hpp"

constexpr CanHelper::CanHelper(AmsState &ams_state_)
    : ams_state(ams_state_)
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