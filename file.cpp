#include <format>
#include "file.h"

File::File(int file_id): RoomEntity(file_id), RegisterIntf("F"), m_index(0)
{
}

void File::reset()
{
    m_index = 0;
}

bool File::isEOF()
{
    return m_index == m_data.size();
}

bool File::isGrabbed()
{
    return m_is_grabbed;
}

void File::grab()
{
    m_is_grabbed = true;
}

void File::drop()
{
    m_is_grabbed = false;
}

void File::write(int value)
{
    if(m_index == m_data.size()){
        m_data.push_back(value);
    } else {
        m_data[m_index] = value;
    }
    m_index++;
}

int File::read() //TODO return std::optional<int>
{
    if(m_index < m_data.size()){
        m_index++;
        return m_data[m_index - 1];
    }
    return 0;
}

void File::seek(int offset)
{
    int new_index = m_index + offset;
    if(new_index > m_data.size()){
        m_index = m_data.size();
    }else if(new_index < 0){
        m_index = 0;
    }
    m_index = new_index;
}

std::string File::toString()
{
    return std::format("File ID:{}, Data:{}, Index:{}, isEOF:{}", getId(), m_data, m_index, isEOF());
}
