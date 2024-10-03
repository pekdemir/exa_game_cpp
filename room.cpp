#include "room.h"
#include <format>

Room::Room(int id, int row, int col): m_id(id), m_row(row), m_col(col), m_slots(row * col, nullptr)
{
}

int Room::getId()
{
    return m_id;
}

void Room::addLink(int id, Room *entity)
{
    m_links[id] = entity;
}

Room *Room::getLink(int link_id)
{
    auto it = m_links.find(link_id);
    if (it != m_links.end()){
        return it->second;
    }
    return nullptr;
}

bool Room::putEntity(RoomEntity *entity)
{
    // find first empty slot
    auto it = std::find(m_slots.begin(), m_slots.end(), nullptr);
    if (it != m_slots.end()){
        *it = entity;
        entity->setRoom(this);
        return true;
    }
    return false;
}

bool Room::removeEntity(RoomEntity *entity)
{
    auto it = std::find(m_slots.begin(), m_slots.end(), entity);
    if (it != m_slots.end()){
        *it = nullptr;
        entity->setRoom(nullptr);
        return true;
    }
    return false;
}

RoomEntity *Room::getEntity(int id)
{
    for (auto entity : m_slots){
        if (entity && entity->getId() == id) return entity;
    }
    return nullptr;
}

std::string Room::toString()
{
    std::string result = std::format("Room ID:{}, Row:{}, Col:{}\n", m_id, m_row, m_col);
    result += "Entities:\n";
    for (auto entity : m_slots){
        if (entity){
            result += entity->toString() + "\n";
        }
    }
    result += "Links:\n";
    for (auto link : m_links){
        result += std::format("Link ID:{} -> Room ID:{}\n", link.first, link.second->getId());
    }
    return result;
}
