#include <format>
#include "room_variable.h"

RoomVariable::RoomVariable(const std::string& room_var_id, bool is_read_only): RoomEntity(room_var_id), m_value{0}, m_is_read_only{is_read_only} {
}

RoomVariable::~RoomVariable()
{
}

int RoomVariable::read()
{
    return m_value;
}

void RoomVariable::write(int value)
{
    if (!m_is_read_only)
        m_value = value;
}

std::string RoomVariable::toString()
{
    return std::format("Variable {}, value:{}, read_only:{}", m_id, m_value, m_is_read_only);
}
